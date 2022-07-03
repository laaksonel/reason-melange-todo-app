
let join = (input: list(string), delimiter: string): string =>
  List.fold_left((acc, input) => acc ++ delimiter ++ input, List.hd(input), List.tl(input));

let decodeTodo = (json): TodoItem.t =>
  Json.Decode.{
    id: json |> field("id", int),
    title: json |> field("title", string),
    completed: json |> field("completed", bool)
  };

// This needs bs-platform dependency, use decco instead
let decodeTodoCollection = (json): TodoItemCollection.t =>
  { items: Json.Decode.list(decodeTodo, json) }

let fetchTodos = () => {
  open Js.Promise;
  // TODO: Error handling
  Fetch.fetch("/todos")
  |> then_(Fetch.Response.json)
  |> then_(json =>
    json
    |> decodeTodoCollection
    |> (todos => resolve(TodoStore.store.dispatch(TodoStore.SetItems(Some(todos)))))
  )
  |> ignore;
};
