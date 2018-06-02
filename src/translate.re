open Parser_flow;
open Ast;
open Belt;

module Yet = {
  type t =
    | Statement
    | Expression
    | Type
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
      },
    );
};

let rec statement =
  fun
  | (
      _,
      Statement.FunctionDeclaration({
        id: Some((_, id)),
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
            |> List.toArray
            |> Js.Array.joinWith(",");
          Printf.sprintf("<%s>", params);
        | None => ""
        };
      let params = List.map(params, pattern);
      let returnType =
        switch (returnType) {
        | Some((_, (_, ty))) => Printf.sprintf(":%s", types(ty))
        | None => ""
        };

      let body: string =
        switch (body) {
        | Function.BodyBlock((_, block)) =>
          block.body
          |> List.map(_, statement)
          |> List.toArray
          |> Js.Array.joinWith("\n")
        | Function.BodyExpression((_, _)) => "expr"
        };

      Printf.sprintf(
        "function %s%s(%s)%s{%s}",
        id,
        typeParameters,
        Js.Array.joinWith(" ", Belt.List.toArray(params)),
        returnType,
        body,
      );
    }
  | (_, Statement.Return({argument: Some(expr)})) =>
    Printf.sprintf("return %s;", expression(expr))
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
  | _ => raise(Yet.Error(Yet.Type))
and pattern =
  fun
  | (_, Pattern.Identifier({name: (_, ident), typeAnnotation, optional: _})) =>
    Printf.sprintf(
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
    Printf.sprintf("%s()", expression(callee))
  | (_, Expression.Member({property, computed: _, _object})) => {
      let mem =
        switch (property) {
        | Expression.Member.PropertyIdentifier((_, ident)) => ident
        | Expression.Member.PropertyExpression(_) => "prop expr"
        };
      Printf.sprintf("%s.%s", expression(_object), mem);
    }
  | (_, Expression.New(_)) => "New"
  | (_, Expression.Identifier((_, name))) => name
  | _ => raise(Yet.Error(Yet.Expression));
