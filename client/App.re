module Container = [%styled.div
  {|
    padding: 5%;
  |}
];

module Text = [%styled.p
  {|
  font-size: 1.875rem;
  text-decoration: none;
  color: #4299E1;
|}
];

[@react.component]
let make = () => {
  let state = TodoStore.store.useStore();

  React.useEffect0(() => {
    TodoService.fetchTodos();
    None;
  });

  <Container>
    <Text>{ React.string("TODOs") }</Text>
    <ul>
      {
        state.items
          |> List.map((item: TodoItem.t) => <TodoListItem key={string_of_int(item.id)} item />)
          |> Array.of_list
          |> React.array
      }
    </ul>
  </Container>
};
