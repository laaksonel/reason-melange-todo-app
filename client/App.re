[@react.component]
let make = () => {
  let state = TodoStore.store.useStore();

  React.useEffect0(() => {
    TodoService.fetchTodos();
    None;
  });

  <div>
    <p>{ React.string("TODO") }</p>
    <ul>
      {
        state.items
          |> List.map((item: TodoItem.t) => <TodoListItem id=item.id />)
          |> Array.of_list
          |> React.array
      }
    </ul>
  </div>
};
