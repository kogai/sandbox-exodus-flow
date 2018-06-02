open Jest;
open Expect;
open! Expect.Operators;

let statement_of_string = content => {
  let (ast, _) =
    Parser_flow.program_file(content, Some(Loc.SourceFile("dummy.js")));
  let (_, statements, _) = ast;
  List.hd(statements);
};

describe("Translate", () => {
  test("can convert function literal", () => {
    let source = "
// @flow
function f(x:number):string {
  return x.toString();
}
";
    source
    |> statement_of_string
    |> Translate.statement
    |> expect
    |> toBe("function f(x:number):string{return x.toString();}");
  });

  test("can convert function with type parameter", () => {
    let source = "
// @flow
function f<T>(x:T):T {
  return x;
}
";
    source
    |> statement_of_string
    |> Translate.statement
    |> expect
    |> toBe("function f<T>(x:T):T{return x;}");
  });
});
