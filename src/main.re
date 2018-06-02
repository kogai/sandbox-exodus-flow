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
        /* body, */
      }),
    ) => {
      let params =
        Belt.List.map(params, ((_loc, param)) =>
          switch (param) {
          | Parser_flow.Ast.Pattern.Identifier({name: (_, ident)}) => ident
          | _ => ""
          }
        );
      let returnType =
        switch (returnType) {
        | Parser_flow.Ast.Type.String => ":string"
        | _ => ""
        };

      Printf.sprintf(
        "function %s(%s)%s",
        id,
        Js.Array.joinWith(" ", Belt.List.toArray(params)),
        returnType,
      );
    }
  | _ => "Not function";

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
