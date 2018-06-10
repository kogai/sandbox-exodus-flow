open Belt;
open Printf;
open Jest;
open Expect;
open! Expect.Operators;

module Prettier = {
  type option = {. "parser": string};

  [@bs.module "prettier"] external format : (string, option) => string = "";
};

let statement_of_string = content => {
  let (ast, _) =
    Parser_flow.program_file(content, Some(Loc.SourceFile("dummy.js")));
  let (_, statements, _) = ast;
  List.headExn(statements)
  |> (
    x =>
      try (Translate.statement(x)) {
      | Translate.Yet.Error(y) => Translate.Yet.string_of_t(y)
      }
  );
};

let tests_dir = "tests";

describe("Translate", () =>
  tests_dir
  |> Node.Fs.readdirSync
  |> Js.Array.filter(path =>
       Js.Re.fromString(".*jsx?$") |> Js.Re.test(path)
     )
  |> List.fromArray
  |> List.forEach(_, path =>
       test(
         sprintf("can translate (%s)", path),
         () => {
           let filename = Filename.chop_extension(path);
           let source =
             Node.Fs.readFileAsUtf8Sync(sprintf("%s/%s", tests_dir, path));
           let expect_result =
             Node.Fs.readFileAsUtf8Sync(
               sprintf("%s/%s.ts", tests_dir, filename),
             );
           source
           |> statement_of_string
           |> Prettier.format(_, {"parser": "typescript"})
           |> expect
           |> toBe(expect_result);
         },
       )
     )
);
