let encodeTodo = (item: TodoItem.t) => {
  open Json.Encode;
  let props = [
    ("id", int(item.id)),
    ("title", string(item.title)),
    ("completed", bool(item.completed)),
  ];

  object_(props);
};

let decodeTodo = (json): TodoItem.t =>
  Json.Decode.{
    id: json |> field("id", int),
    title: json |> field("title", string),
    completed: json |> field("completed", bool),
  };

let decodeTodoCollection = (json): TodoItemCollection.t => {
  items: Json.Decode.list(decodeTodo, json),
};

let fetchTodos = () => {
  Js.Promise.
    // TODO: Error handling
    (
      Fetch.fetch("/todos")
      |> then_(Fetch.Response.json)
      |> then_(json =>
           json
           |> decodeTodoCollection
           |> (
             todos =>
               resolve(
                 TodoStore.store.dispatch(TodoStore.SetItems(Some(todos))),
               )
           )
         )
      |> ignore
    );
};

let toggleItem = (id: int) => {
  open Js.Promise;

  let state = TodoStore.store.getState();
  let selected_item =
    List.find_opt((item: TodoItem.t) => item.id == id, state.items);

  switch (selected_item) {
  | Some(item) =>
    let uri = "todos/" ++ string_of_int(id);
    let newItem = {...item, completed: !item.completed};

    let requestInit =
      Fetch.RequestInit.make(
        ~method_=Put,
        ~body=Fetch.BodyInit.make(newItem |> encodeTodo |> Js.Json.stringify),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      );

    Fetch.fetchWithInit(uri, requestInit)
    |> then_(response => {
         response |> Fetch.Response.ok
           ? {
             TodoStore.store.dispatch(TodoStore.ToggleItem(id));
             resolve(Some(true));
           }
           : {
             Js.log("Failed to update item with id " ++ string_of_int(id));
             resolve(None);
           }
       })
    |> ignore;
  | None => ()
  };
};

let createTodo = (newItem: TodoItem.t) => {
  open Js.Promise;

  let requestInit =
    Fetch.RequestInit.make(
      ~method_=Post,
      ~body=Fetch.BodyInit.make(newItem |> encodeTodo |> Js.Json.stringify),
      ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
      (),
    );

  Fetch.fetchWithInit("todos", requestInit)
  |> then_(Fetch.Response.json)
  |> then_(json =>
       json
       |> decodeTodo
       |> (
         todo => resolve(TodoStore.store.dispatch(TodoStore.AddItem(todo)))
       )
     )
  |> ignore;
};
