module ModalContainer = [%styled.div
  {|
    position: fixed;
    top: 0;
    left: 0;
    bottom: 0;
    right: 0;
    background-color: rgba(0, 0, 0, 0.5);

    display: flex;
    align-items: center;
    justify-content: center;
  |}
];

module ModalContent = [%styled.div
  {|
    background-color: #fff;
    font-family: "Roboto";
    width: 600px;
  |}
];

module ModalHeader = [%styled.div
  {|
    padding: 10px;
    width: 600px;
  |}
];

module ModalBody = [%styled.div
  {|
    padding: 10px;
    border-top: 1px solid #eee;
    border-bottom: 1px solid #eee;
  |}
];

module ModalFooter = [%styled.div
  {|
    display: flex;
    justify-content: end;
  |}
];

let useStyles =
  Mui.Styles.makeStyles({
    "textfieldWidth": ReactDOM.Style.make(~width="100%", ()),
  });

[@react.component]
let make = (~show: bool, ~onCancel: unit => unit, ~onSubmit) =>
  if (show) {
    let classes = useStyles(.);

    let (text, setText) = React.useState(_ => "");

    <ModalContainer onClick={_ => onCancel()}>
      <ModalContent onClick={e => ReactEvent.Mouse.stopPropagation(e)}>
        <ModalHeader> {React.string("Add Todo")} </ModalHeader>
        <ModalBody>
          <Mui.TextField
            variant=`standard
            required=true
            value={Mui.TextField.Value.string(text)}
            onChange={e => {
              let value = ReactEvent.Form.target(e)##value;
              setText(_ => value);
            }}
            classes={Mui.TextField.Classes.make(
              ~root=classes##textfieldWidth,
              (),
            )}
          />
        </ModalBody>
        <ModalFooter>
          <Mui.Button onClick={_ => onSubmit(text)}>
            {React.string("Save")}
          </Mui.Button>
          <Mui.Button onClick={_ => onCancel()}>
            {React.string("Cancel")}
          </Mui.Button>
        </ModalFooter>
      </ModalContent>
    </ModalContainer>;
  } else {
    React.null;
  };
