open ReactHelper;
module Item = {
  [@react.component]
  let make =
    React.forwardRef(
      (
        ~children,
        ~active,
        ~selected,
        ~id=?,
        ~onClick=?,
        ~onFocus=?,
        ~onMouseMove=?,
        ~onPointerLeave=?,
        ~role=?,
        ~style=?,
        ref,
      ) => {
      <li
        ?id
        ?onClick
        ?onFocus
        ?onMouseMove
        ?onPointerLeave
        ?role
        ?style
        ariaSelected=selected
        ref=?{ref |> Js.Nullable.toOption |> Option.map(ReactDOM.Ref.domRef)}
        className={Cn.make([|
          "absolute top-0 left-0 w-full cursor-default p-2",
          active && !selected ? "bg-blue-500 text-white" : "",
          selected ? "bg-red-500 text-white" : "",
        |])}>
        children
      </li>
    });
};

[%%mel.raw
  {|
function getTextWidth(text, font) {
  // re-use canvas object for better performance
  const canvas = getTextWidth.canvas || (getTextWidth.canvas = document.createElement("canvas"));
  const context = canvas.getContext("2d");
  context.font = font;
  const metrics = context.measureText(text);
  return Math.ceil(metrics.width);
}
|}
];

external getTextWidth: (string, string) => int = "getTextWidth";

[@react.component]
let make =
    (
      ~options: array('item),
      ~onSelect: 'item => unit,
      ~initialSelected: option('item)=?,
      ~selectLabel: 'item => string,
      ~itemEqual: ('item, 'item) => bool,
      ~renderItem: 'item => React.element,
      ~renderButton: option('item) => React.element,
      ~initialMaxHeight: int=400,
      ~initialPadding: int=25,
      ~smallViewportHeightPadding: int=5,
    ) => {
  let (open_, setOpen) = React.useState(_ => false);
  let (pointer, setPointer) = React.useState(_ => false);
  let (activeIndex, setActiveIndex) = React.useState(_ => None);
  let (selected, setSelected) = React.useState(_ => initialSelected);

  let (maxHeight, setMaxHeight) = React.useState(_ => initialMaxHeight);
  let (padding, setPadding) = React.useState(_ => initialPadding);

  let (inputValue, setInputValue) = React.useState(_ => "");

  let noResultId = React.useId();
  let buttonId = React.useId();
  let listboxId = React.useId();

  if (!open_ && pointer) {
    setPointer(_ => false);
  };

  React.useEffect0(() => {
    let onResize = _ => {
      let height =
        Webapi.Dom.window |> Window.visualViewport |> VisualViewport.height;
      let newPadding =
        height
        |> Option.map(height =>
             height < initialMaxHeight
               ? smallViewportHeightPadding : initialPadding
           );
      setPadding(_ => Option.value(newPadding, ~default=initialPadding));
    };

    Webapi.Dom.window
    |> Webapi.Dom.Window.addEventListener("resize", onResize);
    Webapi.Dom.window
    |> Window.visualViewport
    |> VisualViewport.addResizeEventListener(onResize);

    Some(
      () => {
        Webapi.Dom.window
        |> Webapi.Dom.Window.removeEventListener("resize", onResize);
        Webapi.Dom.window
        |> Window.visualViewport
        |> VisualViewport.removeResizeEventListner(onResize);
      },
    );
  });

  let listRef = React.useRef([||]);

  let {refs, floatingStyles, context, isPositioned}: FloatingUi.useFloatingReturn =
    FloatingUi.useFloating(
      FloatingUi.useFloatingOptions(
        ~open_,
        ~placement="bottom-start",
        ~whileElementsMounted=FloatingUi.autoUpdate,
        ~onOpenChange=
          value => {
            setOpen(_ => value);
            if (!value) {
              setActiveIndex(_ => None);
            };
          },
        ~middleware=
          FloatingUi.Middleware.(
            [|
              shift(),
              offset(5),
              size(
                sizeOptions(
                  ~padding,
                  ~apply=
                    ({availableHeight, placement, _}) => {
                      let maxHeight =
                        Js.String.startsWith(~prefix="bottom", placement)
                          ? Js.Math.max_int(
                              padding == initialPadding ? 150 : 75,
                              Js.Math.min_int(
                                availableHeight,
                                initialMaxHeight,
                              ),
                            )
                          : Js.Math.max_int(
                              90,
                              Js.Math.min_int(
                                availableHeight,
                                initialMaxHeight,
                              ),
                            );

                      ReactDOM.flushSync(() => {setMaxHeight(_ => maxHeight)});
                    },
                  (),
                ),
              ),
              flip(
                flipOptions(
                  ~padding,
                  ~fallbackStrategy="initialPlacement",
                  (),
                ),
              ),
            |]
          ),
        (),
      ),
    );

  let items =
    React.useMemo2(
      () =>
        options
        |> Js.Array.filter(~f=item => {
             item
             |> selectLabel
             |> Js.String.toLowerCase
             |> Js.String.startsWith(
                  ~prefix=Js.String.toLowerCase(inputValue),
                )
           }),
      (inputValue, options),
    );

  let optionsMaxWidth =
    React.useMemo1(
      () =>
        items
        |> Js.Array.map(~f=selectLabel)
        |> Js.Array.map(~f=label =>
             getTextWidth(label, "normal 16px Segoe UI") + 50
           )
        |> Js.Array.concat(~other=[|0|])  // handle empty array
        |> Js.Math.maxMany_int,
      [|items|],
    );

  let selectedIndex =
    React.useMemo2(
      () => {
        let index =
          (
            selected
            |> Option.map(selected =>
                 Js.Array.findIndex(
                   ~f=item => {itemEqual(selected, item)},
                   items,
                 )
               )
          )
          ->(Option.value(~default=-1));
        index != (-1) ? Some(index) : None;
      },
      (selected, items),
    );

  let scrollRef = React.useRef(Js.Nullable.null);
  let rowVirtualizer =
    TanstackVirtual.Virtualizer.use(
      TanstackVirtual.Virtualizer.options(
        ~count=Js.Array.length(items),
        ~overscan=5,
        ~estimateSize=_ => 35,
        ~getScrollElement=() => scrollRef.current,
        (),
      ),
    );

  let {getReferenceProps, getFloatingProps, _}: FloatingUi.useInteractionsReturn =
    FloatingUi.useInteractions([|
      FloatingUi.useRole(context, ()),
      FloatingUi.useClick(context),
      FloatingUi.useDismiss(context),
    |]);

  let {
    getReferenceProps: getInputProps,
    getFloatingProps: getListFloatingProps,
    getItemProps,
  }: FloatingUi.useInteractionsReturn =
    FloatingUi.useInteractions([|
      FloatingUi.useListNavigation(
        context,
        ~props=
          FloatingUi.useListNavigationOptions(
            ~listRef,
            ~selectedIndex=Js.Nullable.fromOption(selectedIndex),
            ~activeIndex=Js.Nullable.fromOption(activeIndex),
            ~onNavigate=?{
              open_
                ? Some(
                    activeIndex =>
                      setActiveIndex(_ => Js.Nullable.toOption(activeIndex)),
                  )
                : None;
            },
            ~virtual_=true,
            (),
          ),
        (),
      ),
    |]);

  React.useLayoutEffect6(
    () => {
      if (isPositioned && !pointer) {
        if (Option.is_none(activeIndex)
            && Option.is_none(selectedIndex)
            && Js.Array.length(rowVirtualizer.getVirtualItems()) > 0) {
          rowVirtualizer.scrollToIndex(. 0, None);
        };
        if (Option.is_some(activeIndex)) {
          rowVirtualizer.scrollToIndex(. Option.get(activeIndex), None);
        };
      };
      None;
    },
    (rowVirtualizer, isPositioned, activeIndex, pointer, selectedIndex, refs),
  );

  React.useLayoutEffect3(
    () => {
      if (isPositioned && Option.is_some(selectedIndex)) {
        let selectedIndex = Option.get(selectedIndex);
        rowVirtualizer.scrollToIndex(.
          selectedIndex,
          Some(TanstackVirtual.scrollToIndexOptions(~align="start", ())),
        );
        setActiveIndex(_ => Some(selectedIndex));
      };
      None;
    },
    (rowVirtualizer, isPositioned, selectedIndex),
  );

  let floatingProps =
    Js.Obj.assign(
      Obj.magic(getFloatingProps(Some(getListFloatingProps(None)))),
      {"aria-activedescendant": Js.undefined},
    );

  let handleInputChange = (event: React.Event.Form.t) => {
    let target = event |> React.Event.Form.target;
    let value = target##value;
    setInputValue(_ => value);
    setActiveIndex(_ => None);
  };

  let handleInputKeyDown = (event: React.Event.Keyboard.t) =>
    if (React.Event.Keyboard.key(event) == "Enter"
        && Option.is_some(activeIndex)) {
      let activeIndex = Option.get(activeIndex);
      onSelect(items[activeIndex]);
      setSelected(_ => Some(items[activeIndex]));

      setActiveIndex(_ => None);
      setOpen(_ => false);
      setInputValue(_ => "");
    };

  <>
    <Spread props={getReferenceProps(None)}>
      <button
        type_="button"
        id=buttonId
        ref={ReactDOM.Ref.callbackDomRef(refs.setReference)}
        ariaLabel="Choose country">
        {renderButton(selected)}
      </button>
    </Spread>
    <FloatingUi.FloatingPortal>
      {open_
         ? <FloatingUi.FloatingFocusManager context modal=false>
             <Spread props=floatingProps>
               <div
                 className="flex flex-col"
                 style={ReactDOM.Style.combine(
                   floatingStyles,
                   ReactDOM.Style.make(
                     ~maxHeight=string_of_int(maxHeight) ++ "px",
                     (),
                   ),
                 )}
                 ref={ReactDOM.Ref.callbackDomRef(refs.setFloating)}
                 ariaLabelledby=buttonId>
                 <Spread
                   props={getInputProps(
                     Some(
                       ReactDOM.domProps(
                         ~onChange=handleInputChange,
                         ~onKeyDown=handleInputKeyDown,
                         (),
                       ),
                     ),
                   )}>
                   <input
                     value=inputValue
                     type_="text"
                     className="border-2 p-2 outline-none dark:bg-gray-600/50"
                     placeholder="Search country"
                     ariaAutocomplete="list"
                     ariaExpanded=true
                     role="combobox"
                     ariaControls={
                       Js.Array.length(items) == 0 ? noResultId : listboxId
                     }
                   />
                 </Spread>
                 <div
                   ref={ReactDOM.Ref.domRef(scrollRef)}
                   className="overflow-y-auto bg-white/80"
                   style={ReactDOM.Style.make(
                     ~maxHeight=string_of_int(maxHeight) ++ "px",
                     (),
                   )}
                   tabIndex=(-1)>
                   {Js.Array.length(items) == 0
                      ? <p
                          id=noResultId
                          className="m-2"
                          role="region"
                          ariaAtomic=true
                          ariaLive="assertive">
                          {React.string("No item found.")}
                        </p>
                      : React.null}
                   <ul
                     id=listboxId
                     role="listbox"
                     style={ReactDOM.Style.make(
                       ~height=
                         string_of_int(rowVirtualizer.getTotalSize()) ++ "px",
                       ~position="relative",
                       ~minWidth=string_of_int(optionsMaxWidth) ++ "px",
                       ~width="100%",
                       (),
                     )}
                     onKeyDown={_ => {setPointer(_ => false)}}
                     onPointerMove={_ => {setPointer(_ => true)}}>
                     {rowVirtualizer.getVirtualItems()
                      |> Js.Array.map(
                           ~f=(virtualItem: TanstackVirtual.virtualItem) => {
                           let item = items[virtualItem.index];
                           let index = virtualItem.index;
                           let isActive =
                             (
                               activeIndex
                               |> Option.map(activeIndex =>
                                    activeIndex == index
                                  )
                             )
                             ->(Option.value(~default=false));
                           let isSelected =
                             (
                               selectedIndex
                               |> Option.map(selectedIndex =>
                                    selectedIndex == index
                                  )
                             )
                             ->(Option.value(~default=false));

                           <Spread
                             key={virtualItem.key}
                             props={getItemProps(
                               Some(
                                 ReactDOM.domProps(
                                   ~onClick=
                                     _ => {
                                       onSelect(item);
                                       setSelected(_ => Some(item));

                                       setOpen(_ => false);
                                       setActiveIndex(_ => None);
                                       setInputValue(_ => "");
                                     },
                                   (),
                                 ),
                               ),
                             )}>
                             <Item
                               ref={ReactDOM.Ref.callbackDomRef(
                                 Js.Array.unsafe_set(listRef.current, index),
                               )}
                               id={listboxId ++ "-" ++ virtualItem.key}
                               active=isActive
                               selected=isSelected
                               style={ReactDOM.Style.make(
                                 ~height=
                                   string_of_int(virtualItem.size) ++ "px",
                                 ~transform=
                                   "translateY("
                                   ++ string_of_int(virtualItem.start)
                                   ++ "px)",
                                 (),
                               )}>
                               {renderItem(item)}
                             </Item>
                           </Spread>;
                         })
                      |> React.array}
                   </ul>
                 </div>
               </div>
             </Spread>
           </FloatingUi.FloatingFocusManager>
         : React.null}
    </FloatingUi.FloatingPortal>
  </>;
};
