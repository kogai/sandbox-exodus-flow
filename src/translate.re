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
        returnType: Some((_, (_, returnType))),
        body,
      }),
    ) => {
      let params = List.map(params, pattern);
      /* List.map(params, ((_loc, param)) =>
           switch (param) {
           | Pattern.Identifier({name: (_, ident)}) => ident
           | _ => "not implemented"
           }
         ); */
      let returnType =
        switch (returnType) {
        | Type.String => ":string"
        | _ => "not implemented"
        };

      let body: string =
        switch (body) {
        | Function.BodyBlock((_, block)) =>
          block.body
          |> List.map(_, statement)
          |> Belt.List.toArray
          |> Js.Array.joinWith("\n")
        | Function.BodyExpression((_, _)) => "expr"
        };

      Printf.sprintf(
        "function %s(%s)%s{%s}",
        id,
        Js.Array.joinWith(" ", Belt.List.toArray(params)),
        returnType,
        body,
      );
    }
  | (_, Statement.Return({argument: Some(expr)})) =>
    Printf.sprintf("return %s;", expression(expr))
  | _ => "Not implemented(Statement)"
and types =
  fun
  | Type.String => "string"
  | Type.Number => "number"
  | _ => raise(Yet.Error(Yet.Statement))
and pattern =
  fun
  | (_, Pattern.Identifier({name: (_, ident), typeAnnotation, optional: _})) =>
    Printf.sprintf("%s:%s", ident, switch typeAnnotation {
    | Some((_, (_, ty))) => types(ty)
    | None => ""
    })
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
  | _ => "Not implemented(Expression)";
