[%%mel.raw {|import "modern-normalize/modern-normalize.css"|}];

let client = ReactQuery.Client.Provider.createClient();
let component =
  <React.StrictMode>
    <ReactQuery.Client.Provider client> <App /> </ReactQuery.Client.Provider>
  </React.StrictMode>;

let element = ReactDOM.querySelector("#root");
switch (element) {
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, component);
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
};
