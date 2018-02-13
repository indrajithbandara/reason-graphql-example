open Jest;

open ExpectJs;

let () = {
  describe(
    "Query.firstExample",
    () =>
      testPromise(
        "returns a single entry",
        () =>
          GraphQL.run(Schema.schema, "{ firstExample { name, id } }")
          |> Js.Promise.then_(
               (execResult) => Js.Promise.resolve(expect(execResult) |> toMatchSnapshot)
             )
      )
  );
  describe(
    "Query.examples",
    () =>
      testPromise(
        "returns multiple entries",
        () =>
          GraphQL.run(Schema.schema, "{ examples { name, id } }")
          |> Js.Promise.then_(
               (execResult) => Js.Promise.resolve(expect(execResult) |> toMatchSnapshot)
             )
      )
  )
};
