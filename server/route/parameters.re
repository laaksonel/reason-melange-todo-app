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

module Path = {
  module type One = {
    type t
    let of_string : string => t
  }

  module One = {
    module Int = {
      type t = int
      let of_string = int_of_string
    }
  }
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

  module Path = {
    let get_one_param_name = (module S : Route_builder.Specification) => {
      let colon_count = Core.String.count(~f=Core.Char.equal(':'), S.path);
      let () = assert(colon_count == 1);

      let pattern = Str.regexp({re|.*:\([^/]*\)\(/\|$\)|re});
      let () = assert(Str.string_match(pattern, S.path, 0));
      Str.matched_group(1, S.path);
    }

    module One = {
      module Only (Parameters: Path.One, S: Route_builder.Specification) = {
        let name = get_one_param_name (module S)

        let f = req =>
          Parameters.of_string(Opium.Router.param(req, name))
          |> Lwt_result.return
      }
    }
  }
}

