let source = "
// @flow
function f(x:number):string {
  return x.toString();
}
";

let rec translate =
  fun
  | (
      _,
      Parser_flow.Ast.Statement.FunctionDeclaration({
        id: Some((_, id)),
        params: (params, _rest),
        returnType: Some((_, (_, returnType))),
        body,
      }),
    ) => {
      let params =
        Belt.List.map(params, ((_loc, param)) =>
          switch (param) {
          | Parser_flow.Ast.Pattern.Identifier({name: (_, ident)}) => ident
          | _ => "not implemented"
          }
        );
      let returnType =
        switch (returnType) {
        | Parser_flow.Ast.Type.String => ":string"
        | _ => "not implemented"
        };

      let body: string =
        switch (body) {
        | Parser_flow.Ast.Function.BodyBlock((_, block)) =>
          block.body
          |> List.map(translate)
          |> Belt.List.toArray
          |> Js.Array.joinWith("\n")
        | Parser_flow.Ast.Function.BodyExpression((_, _)) => "expr"
        };

      Printf.sprintf(
        "function %s(%s)%s{%s}",
        id,
        Js.Array.joinWith(" ", Belt.List.toArray(params)),
        returnType,
        body,
      );
    }
  | (_, Parser_flow.Ast.Statement.Return({argument: Some(expr)})) =>
    Printf.sprintf("return %s;", translate_expression(expr))
  | _ => "Not implemented"
and translate_expression =
  fun
  | (_, Parser_flow.Ast.Expression.Call({callee, arguments: _})) =>
    Printf.sprintf("%s()", translate_expression(callee))
  | (_, Parser_flow.Ast.Expression.Member({property, computed: _, _object})) => {
      let mem =
        switch (property) {
        | Parser_flow.Ast.Expression.Member.PropertyIdentifier((_, ident)) => ident
        | Parser_flow.Ast.Expression.Member.PropertyExpression(_) => "prop expr"
        };
      Printf.sprintf("%s.%s", translate_expression(_object), mem);
    }
  | (_, Parser_flow.Ast.Expression.New(_)) => "New"
  | (_, Parser_flow.Ast.Expression.Identifier((_, name))) => name
  | _ => "Not implemented(Expression)";

let () = {
  Js.log(source);
  Js.log("");
  let (ast, _) =
    Parser_flow.program_file(source, Some(Loc.SourceFile("./main.re")));

  let (_, statements, _) = ast;
  let ts =
    statements
    |> Belt.List.map(_, translate)
    |> Belt.List.toArray
    |> Js.Array.joinWith("\n");

  Js.log(ts);
};
