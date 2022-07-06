module TodoList = [%styled.div
  {|
    display: flex;
    justify-content: center;
    flex-direction: column;
    align-items: center;
  |}
];

module Topic = [%styled.div
  {|
    font-size: 2rem;
    display: flex;
    justify-content: center;
    color: white;
    font-family: "Roboto";
    padding: 20px 0 20px 0;
  |}
];

module AddCircle = {
  [@bs.module "@material-ui/icons/AddCircle"] [@react.component]
  external make:
    (
      ~color: string=?,
      ~className: string=?,
      ~fontSize: string=?,
      ~htmlColor: string=?
    ) =>
    React.element =
    "default";
};

[@react.component]
let make = () => {
  let state = TodoStore.store.useStore();

  React.useEffect0(() => {
    TodoService.fetchTodos();
    None;
  });

  let buttonClicked = _ => {
    Js.log("FOO");
  };

  <>
    <Topic> {React.string("TODO")} </Topic>
    <Mui.IconButton onClick=buttonClicked>
      <AddCircle htmlColor="#f50057" />
    </Mui.IconButton>
    <TodoList>
      {state.items
       |> List.map((item: TodoItem.t) =>
            <TodoListItem key={string_of_int(item.id)} item />
          )
       |> Array.of_list
       |> React.array}
    </TodoList>
  </>;
};
