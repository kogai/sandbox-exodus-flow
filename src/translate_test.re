open Jest;
open Expect;
open! Expect.Operators;

let statement_of_string = content => {
  let (ast, _) =
    Parser_flow.program_file(content, Some(Loc.SourceFile("dummy.js")));
  let (_, statements, _) = ast;
  List.hd(statements)
  |> (
    x =>
      try (Translate.statement(x)) {
      | Translate.Yet.Error(y) => Translate.Yet.string_of_t(y)
      }
  );
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
    |> expect
    |> toBe("function f(x:number):string{return x.toString();}");
  });

  test("can convert function with type parameter", () => {
    let source = "
// @flow
function id<T>(x:T):T {
  return x;
}
";
    source
    |> statement_of_string
    |> expect
    |> toBe("function id<T>(x:T):T{return x;}");
  });

  test("can convert intersection type", () => {
    let source = "
// @flow
type T = {
  a: number,
  ...{ b: string },
} & { c: boolean};
";
    source
    |> statement_of_string
    |> expect
    |> toBe("type T = {a:number,b:string} & {c:boolean}");
  });
});
