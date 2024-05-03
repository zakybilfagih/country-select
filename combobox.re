open ReactHelper;
open Bindings;
open StyleHelper;

module Item = {
  module Style = {
    let item = [%cx
      {|
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      cursor: default;
      padding: 4px 10px;
      color: $(Colors.Light.textPrimary);
    |}
    ];

    let active = [%cx
      {|
      background-color: $(Colors.Light.backgroundControlHover);
    |}
    ];

    let selected = [%cx
      {|
      background-color: $(Colors.Light.backgroundSelected);
    |}
    ];
  };

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
        ~ariaSetsize=?,
        ~ariaPosinset=?,
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
        ?ariaSetsize
        ?ariaPosinset
        ariaSelected=selected
        ref=?{ref |> Js.Nullable.toOption |> Option.map(ReactDOM.Ref.domRef)}
        className={Cn.make([|
          Style.item,
          textMd,
          selected ? Style.selected : active ? Style.active : "",
        |])}>
        children
      </li>
    });
};

module Style = {
  let floating = [%cx
    {|
    display: flex;
    flex-direction: column;
    box-shadow: 0px 3px 18px 0px #00000026;
    background-color: white;
    border: 1px solid $(Colors.Light.borderFloatingAlpha);
    border-radius: 3px;
    overflow: hidden;
  |}
  ];

  let button = [%cx
    {|
    display: flex;
    gap: 5px;
    align-items: center;
    padding: 4px 10px;
    color: $(Colors.Light.textPrimary);
    background-color: white;
    border: 1px solid $(Colors.Light.borderControlAlpha);
    border-radius: 3px;
  |}
  ];

  let buttonTriangle = [%cx {|
    color: $(Colors.Light.textPrimary);
  |}];

  let searchIconContainer = [%cx
    {|
    position: absolute;
    line-height: 0;
    left: 10px;
    right: auto;
    top: 10px;
  |}
  ];

  let input = [%cx
    {|
    border: none;
    padding: 8px;
    outline: none;
    padding-block: 4px;
    padding-left: 32px;
    padding-right: 10px;
    width: 100%;
    color: black;

    &::placeholder {
      color: $(Colors.Light.textTertiary);
    }
  |}
  ];

  let inputContainer = [%cx
    {|
    position: relative;
    padding-block: 4px;
    border-bottom: 1px solid $(Colors.Light.borderLineAlpha);
  |}
  ];

  let scrollContainer = [%cx
    {|
    padding-block: 4px;
    overflow-y: auto;
    background-color: white;
  |}
  ];

  let listbox = [%cx
    {|
    width: 100%;
    position: relative;
    margin: 0px;
    padding: 0px;
    list-style: none;
  |}
  ];

  let noResult = [%cx
    {|
    margin: 0px;
    padding: 8px 10px;
    color: $(Colors.Light.textPrimary)
  |}
  ];
};

[@react.component]
let make =
    (
      ~options: array('item),
      ~onSelect: option('item) => unit,
      ~selected: option('item),
      ~getItemLabel: 'item => string,
      ~itemEqual: ('item, 'item) => bool,
      ~renderItem: 'item => React.element,
      ~renderButton: option('item) => React.element,
      ~initialMaxHeight: int=400,
      ~initialPadding: int=25,
      ~smallViewportHeightPadding: int=5,
      ~additionalContentWidth: int=0,
      ~buttonAriaLabel: string="Choose item",
      ~noResultText: string="No item found.",
    ) => {
  let (open_, setOpen) = React.useState(_ => false);
  let (pointer, setPointer) = React.useState(_ => false);
  let (activeIndex, setActiveIndex) = React.useState(_ => None);

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
             |> getItemLabel
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
        |> Js.Array.map(~f=getItemLabel)
        |> Js.Array.map(~f=label =>
             TextWidth.get(label, "normal 14px Arial")
             + 42
             + additionalContentWidth
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
        ~estimateSize=_ => 26,
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
      onSelect(Some(items[activeIndex]));

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
        ariaLabel=buttonAriaLabel
        className={Cn.make([|Style.button, textMd|])}>
        {renderButton(selected)}
        <Icon.Triangle className=Style.buttonTriangle />
      </button>
    </Spread>
    {open_
       ? <FloatingUi.FloatingPortal>
           <FloatingUi.FloatingFocusManager context modal=false>
             <Spread props=floatingProps>
               <div
                 className=Style.floating
                 style={ReactDOM.Style.combine(
                   floatingStyles,
                   ReactDOM.Style.make(
                     ~maxHeight=string_of_int(maxHeight) ++ "px",
                     (),
                   ),
                 )}
                 ref={ReactDOM.Ref.callbackDomRef(refs.setFloating)}
                 ariaLabelledby=buttonId>
                 <div className=Style.inputContainer>
                   <div className=Style.searchIconContainer>
                     <Icon.Search />
                   </div>
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
                       className={Cn.make([|Style.input, textMd|])}
                       placeholder="Search"
                       ariaAutocomplete="list"
                       ariaExpanded=true
                       role="combobox"
                       ariaControls={
                         Js.Array.length(items) == 0 ? noResultId : listboxId
                       }
                     />
                   </Spread>
                 </div>
                 <div
                   ref={ReactDOM.Ref.domRef(scrollRef)}
                   className=Style.scrollContainer
                   style={ReactDOM.Style.make(
                     ~maxHeight=string_of_int(maxHeight) ++ "px",
                     (),
                   )}
                   tabIndex=(-1)>
                   {Js.Array.length(items) == 0
                      ? <p
                          id=noResultId
                          className={Cn.make([|Style.noResult, textMd|])}
                          role="region"
                          ariaAtomic=true
                          ariaLive="assertive">
                          {React.string(noResultText)}
                        </p>
                      : React.null}
                   <ul
                     id=listboxId
                     role="listbox"
                     className=Style.listbox
                     style={ReactDOM.Style.make(
                       ~height=
                         string_of_int(rowVirtualizer.getTotalSize()) ++ "px",
                       ~minWidth=string_of_int(optionsMaxWidth) ++ "px",
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
                             Option.value(
                               activeIndex |> Option.map(Int.equal(index)),
                               ~default=false,
                             );
                           let isSelected =
                             Option.value(
                               selectedIndex |> Option.map(Int.equal(index)),
                               ~default=false,
                             );

                           <Spread
                             key={virtualItem.key}
                             props={getItemProps(
                               Some(
                                 ReactDOM.domProps(
                                   ~onClick=
                                     _ => {
                                       onSelect(Some(item));

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
                               id={listboxId ++ virtualItem.key}
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
                               )}
                               ariaSetsize={Js.Array.length(items)}
                               ariaPosinset={virtualItem.index + 1}>
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
         </FloatingUi.FloatingPortal>
       : React.null}
  </>;
};
