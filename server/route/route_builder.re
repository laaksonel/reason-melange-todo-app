open Utils;
open Methods;
open Lwt_result.Infix;

module type Specification = {
  let verb : verb
  let path : string

  module Parameters {
    type t;
  };

  module Responses {
    type t;
  };
}

let handle = (req, input_f, impl_f, output_f) => {
  // req has the ref to db
  // Input_f only converts json to domain model
  /* let pool = req.Request.env */
  /*   |> Opium.Context.find_exn(Db.key); */

  /* let run_query = q => Db.query_pool(q, pool); */

  let%lwt impl_output = req |> input_f >>= (x) => impl_f(x); // We need to pass db reference here

  switch impl_output {
  | Ok(y) => output_f(y)
  | Error(_) => respond_error()
  }
}

let specification_to_route = (module X: Specification, input_f, impl_f, output_f) => {
  let route = verb_to_route(X.verb)

  let h = (req) => {
    let result = handle(req, input_f, impl_f, output_f);
    try (result) {
    | _ => respond_error()
    }
  }

  route(X.path, h)
}
