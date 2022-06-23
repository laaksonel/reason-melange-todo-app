open Utils;
open Route_builder;

module type Json = {
  type t

  let yojson_of_t : t => Yojson.Safe.t
}

module type Json_list = Json

module type Json_opt = Json

module Json_list (Some_json : Json) = Some_json

module Json_opt (Some_json : Json) = Some_json

module type No_content = {
  type t = unit
}

module No_content = Unit

module Created = {
  module type Int = {
    type t = int
  }

  module Int = Int
}

module type String = {
  type t = string;
}

module String = {
  type t = string;
}

module Make = {
  module String (Responses : String) = {
    let f = (s) => `String(s) |> respond
  }

  module Created = {
    module Int (Responses : Created.Int, S : Specification) = {
      let f = id => {
        let location = Printf.sprintf("%s/%d", S.path, id);
        let headers = Httpaf.Headers.of_list([ ("Location", location) ]);
        `String("") |> respond(~headers, ~status=`Created)
      }
    }
  }

  module Json (Responses : Json) = {
    let f = (content) => `Json(content |> Responses.yojson_of_t) |> respond
  }

  module Json_list (Responses : Json_list) = {
    let f = (content) => {
      let list_of_json = List.map(Responses.yojson_of_t, content);
      let json_of_list = `List(list_of_json);
      `Json(json_of_list) |> respond
    }
  }

  module Json_opt (Responses : Json_opt) = {
    let f = (~status=`Not_found, content_opt) => {
      switch content_opt {
      | Some(content) => `Json(content |> Responses.yojson_of_t) |> respond
      | None => `String("") |> respond(~status)
      }
    }
  }

  module No_content (Responses : No_content) = {
    let f () = `String("") |> respond(~status=`No_content);
  }
}
