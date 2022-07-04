module Container = [%styled.div
  {|
    background-color: #1e1e1e;
    margin: 0 auto;
    width: 600px;
    border-radius: 10px;
  |}
];

module Text = [%styled.p
  {|
    padding: 20px 30px;
    color: white;
    font-family: "Roboto";
  |}
];

let toggleCompletetion = (e: ReactEvent.Form.t) => {
  let checked = ReactEvent.Form.target(e)##checked;
  Js.log(checked);
};

[@react.component]
let make = (~item: TodoItem.t) => {
  <Container>
    <Text>
      { React.string(item.title) }
    </Text>
    <Mui.Checkbox onChange=toggleCompletetion />
  </Container>
};
