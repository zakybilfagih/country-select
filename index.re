open Bindings;

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
  ViteReactSSG.makeSinglePageWithOptions(
    <Main />,
    ~setupFn=() => (),
    ~options=
      ViteReactSSG.makeSinglePageOptions(
        ~getStyleCollector=emotionStyleCollector,
        (),
      ),
    (),
  );
