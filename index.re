[%%mel.raw {|import "./index.css"|}];

let element = ReactDOM.querySelector("#root");
let component = <React.StrictMode> <App /> </React.StrictMode>;
switch (element) {
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, component);
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
};
