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
    background-color: #fff;
    width: 600px;
  |}
];

let useStyles = Mui.Styles.makeStyles({
  "textfieldWidth": ReactDOM.Style.make(~width="100%", ())
});

[@react.component]
let make = (~show: bool, ~onCancel: unit => unit, ~onSubmit) =>
  if (show) {
    let classes = useStyles(.);

    <ModalContainer>
      <ModalContent>
        <ModalHeader> {React.string("Add Todo")} </ModalHeader>
        <ModalBody>
          <Mui.TextField
            variant=`standard
            required={true}
            classes={Mui.TextField.Classes.make(~root=classes##textfieldWidth, ())}
          />
        </ModalBody>
        <ModalFooter>
          <Mui.Button onClick={_ => onCancel()}>
            {React.string("Cancel")}
          </Mui.Button>
          <Mui.Button onClick={_ => onSubmit()}>
            {React.string("Save")}
          </Mui.Button>
        </ModalFooter>
      </ModalContent>
    </ModalContainer>;
  } else {
    React.null;
  };
