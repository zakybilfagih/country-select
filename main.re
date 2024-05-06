[%%mel.raw {|import "modern-normalize/modern-normalize.css"|}];
let client = ReactQuery.Client.Provider.createClient();
[@react.component]
let make = () => {
  <React.StrictMode>
    <Bindings.ViteReactSSG.Head>
      <title>{React.string("Country Select")}</title>
    </Bindings.ViteReactSSG.Head>
    <ReactQuery.Client.Provider client> <App /> </ReactQuery.Client.Provider>
  </React.StrictMode>;
};
