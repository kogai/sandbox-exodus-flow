let source = "
// @flow
function f(x:number):string {
  return x.toString();
}
";

let () = {
  Js.log(source);
  Js.log("");
  let (ast, _) =
    Parser_flow.program_file(source, Some(Loc.SourceFile("./main.re")));
  let (_, statements, _) = ast;
  let ts =
    Belt.List.map(
      statements,
      st => {
        switch (st) {
        | (_, Parser_flow.Ast.Statement.FunctionDeclaration({ id: Some((_, id)) })) => {
          Js.log(id);
          Printf.sprintf("It's fucntion [%s]", id)
        }
        | _ => "Not function"
        }
      },
    );
  
  Js.log(Js.Array.joinWith("\n", Belt.List.toArray(ts)))
};
