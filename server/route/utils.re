open Opium;

type my_body = [ `Json(Yojson.Safe.t) | `String(string) | `Empty ]

let to_body = (x) => switch x {
  | `String s => Body.of_string(s)
  | `Empty => Body.empty
  | `Json j => j |> Yojson.Safe.to_string |> Body.of_string
};

let respond = (~headers=?, ~status=?, body) => {
  Response.make(~headers?, ~status?, ~body=(to_body(body)), ()) |> Lwt.return
}

let respond_error = () =>
  respond(~status=`Internal_server_error, `String("FOO"))

