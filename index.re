open Bindings;

[%%mel.raw {|import "modern-normalize/modern-normalize.css"|}];

let client = ReactQuery.Client.Provider.createClient();

let component =
  <React.StrictMode>
    <ViteReactSSG.Head>
      <title> {React.string("Country Select")} </title>
    </ViteReactSSG.Head>
    <ReactQuery.Client.Provider client> <App /> </ReactQuery.Client.Provider>
  </React.StrictMode>;

let emotionStyleCollector = [%mel.raw
  {|
  async function () {
    const { renderToString } = await import('react-dom/server');
    const { extractCriticalToChunks, constructStyleTagsFromChunks } = await import('@emotion/server');

    let renderedApp;

    return {
      collect: (app) => {
        renderedApp = app
        return app
      },
      toString: () => {
        if (!renderedApp) {
          return ""
        }

        let markup = renderToString(renderedApp);
        let chunks = extractCriticalToChunks(markup)
        return constructStyleTagsFromChunks(chunks)
      }
    }
  }
|}
];

let createRoot =
  ViteReactSSG.SinglePage.makeWithOptions(
    component,
    ~options=
      ViteReactSSG.SinglePage.makeOptions(
        ~getStyleCollector=emotionStyleCollector,
        (),
      ),
    (),
  );
