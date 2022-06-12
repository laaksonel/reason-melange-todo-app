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

let handlers = List.flatten([ hello_world ])
