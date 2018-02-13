open Express;

/* let raiseIfNone = */
/*   fun */
/*   | Some(value) => value */
/*   | None => failwith("Body is none"); */

let onListen = (port, e) =>
  switch e {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1)
  | _ => Js.log @@ "Listening at http://127.0.0.1:" ++ string_of_int(port)
  };

let makeSuccessJson = () => {
  let json = Js.Dict.empty();
  Js.Dict.set(json, "success", Js.Json.boolean(Js.true_));
  Js.Json.object_(json)
};

module Body = {
  type payload = {. "number": int};
  let jsonDecoder = (json) => Json.Decode.({"number": json |> field("number", int)});
  let urlEncodedDecoder = (dict) => {"number": Js.Dict.unsafeGet(dict, "number") |> int_of_string};
  let encoder = (body) => Json.Encode.(object_([("number", body##number |> int)]));
  };

let app = express();

App.useOnPath(
  app,
  ~path="/",
  {
    let options = Static.defaultOptions();
    Static.make("public", options) |> Static.asMiddleware
    }
);

let graphqlMiddleware = ApolloServerExpress.createGraphQLExpressMiddleware(Schema.schema);
let graphiqlMiddleware = ApolloServerExpress.createGraphiQLExpressMiddleware("/graphql");

[@bs.module "body-parser"] external bodyParserJson : unit => Express.Middleware.t = "json";

App.use(app, bodyParserJson());
App.useOnPath(app, graphqlMiddleware, ~path="/graphql");
App.useOnPath(app, graphiqlMiddleware, ~path="/graphiql");

App.listen(app, ~onListen=onListen(3000), ());
