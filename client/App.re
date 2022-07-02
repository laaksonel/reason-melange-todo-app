[@react.component]
let make = () => {
  let state = TodoStore.store.useStore();

  React.useEffect0(() => {
    TodoService.fetchTodos();
    None;
  });

  <div>
    <p>{ React.string("TODOs") }</p>
    <ul>
      {
        state.items
          |> List.map((item: TodoItem.t) => <TodoListItem item=item />)
          |> Array.of_list
          |> React.array
      }
    </ul>
  </div>
};
