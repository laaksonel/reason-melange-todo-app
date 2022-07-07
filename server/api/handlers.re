open Route;

module Hello = {
  let verb = Methods.Get;
  let path = "/";

  module Parameters = Parameters.None;
  module Responses = Responses.String;
};

let hello_world = {
  module P = Parameters.Make.None(Hello.Parameters);
  module R = Responses.Make.String(Hello.Responses);
  let hello =
    Route_builder.specification_to_route(
      (module Hello),
      P.f,
      Controllers.Hello_world.hello,
      R.f,
    );

  [hello];
};

module Todo = (Db: (module type of Db)) => {
  let index = {
    module P = Parameters.Make.None(Todo.Index.Parameters);
    module R = Responses.Make.Json_list(Todo.Index.Responses);

    Route_builder.specification_to_route(
      (module Todo.Index),
      P.f,
      Controllers.Todo.get_todos(Db.run_query),
      R.f,
    );
  };

  let create = {
    module P = Parameters.Make.Json.Only(Todo.Create.Parameters);
    module R = Responses.Make.Json(Todo.Create.Responses);

    Route_builder.specification_to_route(
      (module Todo.Create),
      P.f,
      Controllers.Todo.create(Db.run_query),
      R.f,
    );
  };

  let update = {
    module P = Parameters.Make.Json.Only(Todo.Update.Parameters);
    module R = Responses.Make.No_content(Todo.Update.Responses);

    Route_builder.specification_to_route(
      (module Todo.Update),
      P.f,
      Controllers.Todo.update(Db.run_query),
      R.f,
    );
  };

  let show = {
    module P = Parameters.Make.Path.One.Only(Todo.Show.Parameters, Todo.Show);
    module R = Responses.Make.Json_opt(Todo.Show.Responses);

    Route_builder.specification_to_route(
      (module Todo.Show),
      P.f,
      Controllers.Todo.show(Db.run_query),
      R.f,
    );
  };

  let endpoints = [index, create, update, show];
};
