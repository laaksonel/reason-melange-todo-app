module type None = {
  type t = unit;
}

module None = {
  type t = unit;
}

let try_with_json_error = f => {
  try (f()) {
  | Yojson.Json_error(message) => Error (`Json (`Parsing(message))) |> Lwt.return
  | Ppx_yojson_conv_lib.Yojson_conv.Of_yojson_error(e, yojson) =>
      Error (`Json(`Conversion (e, yojson))) |> Lwt.return
  }
}


module type Json = {
  type t
  let t_of_yojson : Yojson.Safe.t => t
}

module Make = {
  module None (Parameters : None) = {
    let f = (_req) => () |> Lwt_result.return;
  }

  module Json = {
    module Only (Parameters : Json) = {
      let f = (req) => {
        try_with_json_error (fun () => {
          let%lwt json = Opium.Request.to_json_exn(req);
          Ok(Parameters.t_of_yojson(json)) |> Lwt.return
        })
      }
    }
  }
}

module Path = {
  module type One = {
    type t
    let of_string : string => t
  }

  module One = {
    module Int = {
      type t = int
    }
  }
}
