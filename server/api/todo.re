open Route;

let base_path = "/todos"

module Create = {
  let verb = Methods.Post
  let path = base_path

  module Parameters = Db.Todo.No_id
  module Responses = Responses.Created.Int
}

module Index = {
  let verb = Methods.Get
  let path = base_path

  module Parameters = Parameters.None
  module Responses = Responses.Json_list (Db.Todo)
}

module Show = {
  let verb = Methods.Get
  let path = base_path ++ "/:id"

  module Parameters = Parameters.Path.One.Int
  module Responses = Responses.Json_opt (Db.Todo)
}

module Update = {
  let verb = Methods.Put
  let path = base_path

  module Parameters = Db.Todo
  module Responses = Responses.No_content
}

module Destroy = {
  let verb = Methods.Delete
  let path = base_path ++ "/:id"

  module Parameters = Parameters.Path.One.Int
  module Responses = Responses.No_content
}
