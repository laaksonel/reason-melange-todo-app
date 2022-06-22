open Route;

module Hello {
  let verb = Methods.Get;
  let path = "/";

  module Parameters = Parameters.None;
  module Responses = Responses.String;
}

let hello_world = {
  let module P = Parameters.Make.None(Hello.Parameters);
  let module R = Responses.Make.String(Hello.Responses);
  let hello = Route_builder.specification_to_route((module Hello), P.f, Controllers.Hello_world.hello, R.f);

  [ hello ]
}

module Todo (Db: (module type of Db)) {
  let create = {
    let module P = Parameters.Make.Json.Only (Todo.Create.Parameters);
    let module R = Responses.Make.Created.Int (Todo.Create.Responses, Todo.Create);

    Route_builder.specification_to_route((module Todo.Create), P.f, Controllers.Todo.create(Db.run_query), R.f);
  };

  let index = {
    let module P = Parameters.Make.None(Todo.Index.Parameters);
    let module R = Responses.Make.Json_list(Todo.Index.Responses);

    Route_builder.specification_to_route((module Todo.Index), P.f, Controllers.Todo.index(Db.run_query), R.f);
  };

  let endpoints = [ create, index ];
}

