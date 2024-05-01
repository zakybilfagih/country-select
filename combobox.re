//import {
//  autoUpdate,
//  flip,
//  FloatingFocusManager,
//  FloatingPortal,
//  offset,
//  size,
//  useDismiss,
//  useFloating,
//  useFocus,
//  useInteractions,
//  useListNavigation,
//  useRole,
//} from '@floating-ui/react';
//import classNames from 'classnames';
//import {useEffect} from 'react';
//import {forwardRef, useRef, useState} from 'react';
//
//const Item = forwardRef(function Item(
//  {children, active, ...rest},
//  ref,
//) {
//  return (
//    <li
//      ref={ref}
//      className={classNames(
//        'cursor-default scroll-my-1 rounded-md p-2',
//        {
//          'bg-blue-500 text-white': active,
//        },
//      )}
//      {...rest}
//    >
//      {children}
//    </li>
//  );
//});
//
//export function ComboboxDemo() {
//  const [open, setOpen] = useState(false);
//  const [inputValue, setInputValue] = useState('');
//  const [activeIndex, setActiveIndex] = useState(null);
//  const [padding, setPadding] = useState(25);
//
//  useEffect(() => {
//    function onResize() {
//      setPadding(window.visualViewport?.height < 400 ? 5 : 25);
//    }
//
//    window.addEventListener('resize', onResize);
//    window.visualViewport?.addEventListener('resize', onResize);
//    return () => {
//      window.removeEventListener('resize', onResize);
//      window.visualViewport?.removeEventListener(
//        'resize',
//        onResize,
//      );
//    };
//  }, []);
//
//  const listRef = useRef([]);
//
//  const {refs, floatingStyles, context} = useFloating({
//    open,
//    whileElementsMounted: autoUpdate,
//    onOpenChange: setOpen,
//    middleware: [
//      offset(5),
//      size({
//        apply({rects, elements, availableHeight, placement}) {
//          Object.assign(elements.floating.style, {
//            maxHeight:
//              placement === 'bottom'
//                ? `${Math.max(
//                    padding === 25 ? 150 : 75,
//                    availableHeight,
//                  )}px`
//                : `${Math.max(50, availableHeight)}px`,
//            width: `${rects.reference.width}px`,
//          });
//        },
//        padding,
//      }),
//      flip({padding, fallbackStrategy: 'initialPlacement'}),
//    ],
//  });
//
//  const role = useRole(context, {role: 'combobox'});
//  const focus = useFocus(context, {
//    enabled: inputValue.length > 0,
//  });
//  const dismiss = useDismiss(context);
//  const navigation = useListNavigation(context, {
//    listRef,
//    activeIndex,
//    onNavigate: setActiveIndex,
//    virtual: true,
//    loop: true,
//    allowEscape: true,
//  });
//
//  const {getReferenceProps, getFloatingProps, getItemProps} =
//    useInteractions([role, dismiss, navigation, focus]);
//
//  function onChange(event) {
//    const value = event.target.value;
//    setInputValue(value);
//
//    if (value) {
//      setOpen(true);
//    } else {
//      setOpen(false);
//    }
//  }
//
//  const items = fruits.filter((item) =>
//    item.toLowerCase().startsWith(inputValue.toLowerCase()),
//  );
//
//  return (
//    <>
//      <input
//        ref={refs.setReference}
//        value={inputValue}
//        className="rounded border-2 border-gray-300 p-2 outline-none focus:border-blue-500 dark:border-gray-500 dark:bg-gray-600/50"
//        placeholder="Enter balloon flavor"
//        aria-autocomplete="list"
//        aria-labelledby={
//          open && items.length === 0
//            ? 'combobox-no-results'
//            : undefined
//        }
//        {...getReferenceProps({
//          onChange,
//          onKeyDown(event) {
//            if (
//              event.key === 'Enter' &&
//              activeIndex != null &&
//              items[activeIndex]
//            ) {
//              setInputValue(items[activeIndex]);
//              setActiveIndex(null);
//              setOpen(false);
//            }
//          },
//        })}
//      />
//      {open && (
//        <FloatingPortal>
//          <FloatingFocusManager
//            context={context}
//            initialFocus={-1}
//            visuallyHiddenDismiss
//          >
//            <div
//              ref={refs.setFloating}
//              className="z-10 max-h-[20rem] overflow-y-auto rounded-lg border border-slate-900/5 bg-white/80 bg-clip-padding p-1 text-left shadow-lg outline-none backdrop-blur-lg dark:bg-gray-600/80"
//              style={floatingStyles}
//              {...getFloatingProps()}
//            >
//              {items.length === 0 && (
//                <p
//                  className="m-2"
//                  id="combobox-no-results"
//                  role="region"
//                  aria-atomic="true"
//                  aria-live="assertive"
//                >
//                  No flavors found.
//                </p>
//              )}
//              {items.map((item, index) => (
//                <Item
//                  key={item}
//                  ref={(node) => {
//                    listRef.current[index] = node;
//                  }}
//                  active={activeIndex === index}
//                  {...getItemProps({
//                    onClick() {
//                      setInputValue(item);
//                      setOpen(false);
//                    },
//                  })}
//                >
//                  {item}
//                </Item>
//              ))}
//            </div>
//          </FloatingFocusManager>
//        </FloatingPortal>
//      )}
//    </>
//  );
//}

[@react.component]
let make = () => {
  let (open_, setOpen) = React.useState(_ => false);
  let (padding, _setPadding) = React.useState(_ => 25);

  let _rett =
    FloatingUi.useFloating(
      FloatingUi.useFloatingOptions(
        ~open_,
        ~whileElementsMounted=FloatingUi.autoUpdate,
        ~onOpenChange=value => setOpen(_ => value),
        ~middleware=[|
          FloatingUi.Middleware.offset(5),
          FloatingUi.Middleware.size(
            FloatingUi.Middleware.sizeOptions(
              ~padding=25,
              ~apply=
                ({rects, elements, availableHeight, placement}) => {
                  let maxHeight =
                    placement == "bottom"
                      ? Js.Math.max_int(
                          padding == 25 ? 150 : 75,
                          availableHeight,
                        )
                      : Js.Math.max_int(50, availableHeight);
                  let width = rects.reference.width;

                  Js.Obj.assign(
                    Obj.magic(
                      Webapi.Dom.HtmlElement.style(elements.floating),
                    ),
                    {
                      "maxHeight": string_of_int(maxHeight) ++ "px",
                      "width": string_of_int(width) ++ "px",
                    },
                  )
                  |> ignore;
                },
              (),
            ),
          ),
          FloatingUi.Middleware.flip(
            FloatingUi.Middleware.flipOptions(
              ~padding,
              ~fallbackStrategy="initialPlacement",
              (),
            ),
          ),
        |],
        (),
      ),
    );
  React.null;
};
