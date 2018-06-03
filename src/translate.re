open Parser_flow;
open Ast;
open Belt;
open Printf;

module Yet = {
  type t =
    | Statement
    | Expression
    | Type
    | Property
    | ExpressionKey
    | Pattern;
  exception Error(t);
  let string_of_t = kind =>
    Printf.sprintf(
      "Not implemented yet, %s",
      switch (kind) {
      | Statement => "Statement"
      | Expression => "Expression"
      | Type => "Type"
      | Pattern => "Pattern"
      | ExpressionKey => "ExpressionKey"
      | Property => "Property"
      },
    );
};

let string_of_identifier = ((_, id)) => id;
let string_of_list = (xs, sep) =>
  xs |> List.toArray |> Js.Array.joinWith(sep);

let rec statement =
  fun
  | (
      _,
      Statement.FunctionDeclaration({
        id: Some(id),
        params: (params, _rest),
        returnType,
        body,
        async: _,
        generator: _,
        predicate: _,
        expression: _,
        typeParameters,
      }),
    ) => {
      let typeParameters =
        switch (typeParameters) {
        | Some((_, annot)) =>
          let params =
            List.map(
              annot.params, ((_, {name, bound: _, variance: _, default: _})) =>
              name
            )
            |> string_of_list(_, ",");
          sprintf("<%s>", params);
        | None => ""
        };
      let params = List.map(params, pattern);
      let returnType =
        switch (returnType) {
        | Some((_, (_, ty))) => sprintf(":%s", types(ty))
        | None => ""
        };

      let body: string =
        switch (body) {
        | Function.BodyBlock((_, block)) =>
          block.body |> List.map(_, statement) |> string_of_list(_, "\n")
        | Function.BodyExpression((_, _)) => "expr"
        };

      sprintf(
        "function %s%s(%s)%s{%s}",
        string_of_identifier(id),
        typeParameters,
        string_of_list(params, " "),
        returnType,
        body,
      );
    }
  | (_, Statement.Return({argument: Some(expr)})) =>
    sprintf("return %s;", expression(expr))
  | (_, Statement.TypeAlias({id, typeParameters: _, right: (_, right)})) =>
    sprintf("type %s = %s", string_of_identifier(id), types(right))
  | _ => raise(Yet.Error(Yet.Statement))
and types =
  fun
  | Type.String => "string"
  | Type.Number => "number"
  | Type.Generic({id, typeParameters: _}) => {
      let id =
        switch (id) {
        | Type.Generic.Identifier.Unqualified((_, id)) => id
        | Type.Generic.Identifier.Qualified(_) => "qualified"
        };
      id;
    }
  | Type.Intersection(x, y, xs) =>
    List.concat([x, y], xs)
    |> List.map(_, ((_, ty)) => types(ty))
    |> string_of_list(_, "} & {")
    |> sprintf("{%s}")
  | Type.Object({exact: _, properties}) =>
    List.map(properties, property) |> string_of_list(_, ",")
  | _ => raise(Yet.Error(Yet.Type))
and property =
  fun
  | Type.Object.Property((
      _,
      {key, value, optional: _, static: _, _method: _, variance: _},
    )) =>
    sprintf("%s", expression_object_property_key(key))
  | Type.Object.SpreadProperty(_) => "SpreadProperty"
  | _ => raise(Yet.Error(Yet.Property))
and expression_object_property_key =
  fun
  | _ => raise(Yet.Error(Yet.ExpressionKey))
and pattern =
  fun
  | (_, Pattern.Identifier({name: (_, ident), typeAnnotation, optional: _})) =>
    sprintf(
      "%s:%s",
      ident,
      switch (typeAnnotation) {
      | Some((_, (_, ty))) => types(ty)
      | None => ""
      },
    )
  | _ => raise(Yet.Error(Yet.Statement))
and expression =
  fun
  | (_, Expression.Call({callee, arguments: _})) =>
    sprintf("%s()", expression(callee))
  | (_, Expression.Member({property, computed: _, _object})) => {
      let mem =
        switch (property) {
        | Expression.Member.PropertyIdentifier((_, ident)) => ident
        | Expression.Member.PropertyExpression(_) => "prop expr"
        };
      sprintf("%s.%s", expression(_object), mem);
    }
  | (_, Expression.New(_)) => "New"
  | (_, Expression.Identifier((_, name))) => name
  | _ => raise(Yet.Error(Yet.Expression));
