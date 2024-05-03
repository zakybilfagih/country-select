let canvas = ref(None);

let get = (text, font) => {
  let canvas =
    switch (canvas^) {
    | Some(canvas) => canvas
    | None =>
      canvas :=
        Some(
          Webapi.Dom.Document.createElement("canvas", Webapi.Dom.document),
        );
      Option.get(canvas^);
    };

  let context = Webapi.Canvas.CanvasElement.getContext2d(canvas);
  Webapi.Canvas.Canvas2d.font(context, font);
  let metrics = Webapi.Canvas.Canvas2d.measureText(text, context);
  Js.Math.ceil_float(Webapi.Canvas.Canvas2d.width(metrics)) |> int_of_float;
};
