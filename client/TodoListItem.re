module ItemContainer = [%styled.div
  {|
    background-color: #1e1e1e;
    width: 600px;
    border-radius: 10px;
    justify-content: space-between;
    display: flex;
    padding-left: 10px;
    margin-bottom: 10px;
  |}
];

module Description = [%styled.div {|
    display: flex;
  |}];

module DescriptionText = [%styled.p
  {|
    color: white;
    font-family: "Roboto";
    word-break: break-all;
  |}
];

module CompletedCheckbox = Mui.Checkbox;

let toggleCompletetion = (id, e: ReactEvent.Form.t) => {
  let checked = ReactEvent.Form.target(e)##checked;
  Js.log(string_of_int(id) ++ " -> " ++ checked);
};

[@react.component]
let make = (~item: TodoItem.t) => {
  <ItemContainer>
    <DescriptionText> {React.string(item.title)} </DescriptionText>
    <CompletedCheckbox onChange={toggleCompletetion(item.id)} />
  </ItemContainer>;
};
