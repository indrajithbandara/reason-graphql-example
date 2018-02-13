open Express;

let onListen = (port, e) =>
  switch e {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1)
  | _ => Js.log @@ "Listening at http://127.0.0.1:" ++ string_of_int(port)
  };


let app = express();

let graphqlMiddleware = ApolloServerExpress.createGraphQLExpressMiddleware(Schema.schema);
let graphiqlMiddleware = ApolloServerExpress.createGraphiQLExpressMiddleware("/graphql");

[@bs.module "body-parser"] external bodyParserJson : unit => Express.Middleware.t = "json";

App.use(app, bodyParserJson());
App.useOnPath(app, graphqlMiddleware, ~path="/graphql");
App.useOnPath(app, graphiqlMiddleware, ~path="/graphiql");

App.listen(app, ~onListen=onListen(3000), ());
