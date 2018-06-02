open Belt;

let source = "
// @flow
function f(x:number):string {
  return x.toString();
}
";

let () = {
  let (ast, _) =
    Parser_flow.program_file(source, Some(Loc.SourceFile("./main.re")));

  let (_, statements, _) = ast;
  let ts =
    statements
    |> List.map(_, Translate.statement)
    |> List.toArray
    |> Js.Array.joinWith("\n");

  Js.log(ts);
};
