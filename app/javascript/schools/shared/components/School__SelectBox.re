[@bs.config {jsx: 3}];

let str = React.string;

[@react.component]
let make = (~items, ~multiSelectCB) => {
  let (searchKey, setSearchKey) = React.useState(() => "");
  let selectedList =
    items |> List.filter(((_, _, selected)) => selected == true);
  let nonSelectedList =
    items |> List.filter(((_, _, selected)) => selected == false);
  let filteredList =
    switch (nonSelectedList) {
    | [] => []
    | someList =>
      someList
      |> List.filter(((_key, value, _)) =>
           Js.String.includes(
             String.lowercase(searchKey),
             String.lowercase(value),
           )
         )
    };
  <div>
    {
      selectedList |> List.length > 0 ?
        selectedList
        |> List.rev
        |> List.map(((_key, value, _)) =>
             <div
               key={_key |> string_of_int}
               className="select-list__item-selected flex items-center justify-between bg-grey-lightest text-xs text-grey-dark border rounded p-3 mb-2">
               {value |> str}
               <button
                 onClick={
                   _event => {
                     ReactEvent.Mouse.preventDefault(_event);
                     setSearchKey(_ => "");
                     multiSelectCB(_key, value, false);
                   }
                 }>
                 <Icon kind=Icon.Delete size="4" opacity=75 />
               </button>
             </div>
           )
        |> Array.of_list
        |> React.array :
        <div
          className="select-list__item-selected flex items-center justify-between bg-grey-lightest text-xs text-grey-dark border rounded p-3 mb-2">
          {"None Selected" |> str}
        </div>
    }
    {
      nonSelectedList |> List.length > 0 ?
        <div className="flex relative">
          <div
            className="select-list__group bg-white border rounded rounded-t-none shadow pb-2 w-full">
            {
              nonSelectedList |> List.length > 3 ?
                <div className="px-3 pt-3 pb-2">
                  <input
                    className="appearance-none bg-transparent border-b w-full text-grey-darker pb-3 px-2 pl-0 leading-tight focus:outline-none"
                    type_="text"
                    placeholder="Type to Search"
                    onChange={
                      event =>
                        setSearchKey(ReactEvent.Form.target(event)##value)
                    }
                  />
                </div> :
                React.null
            }
            <div
              className={
                nonSelectedList |> List.length > 3 ?
                  "h-24 overflow-y-scroll" : ""
              }>
              {
                filteredList
                |> List.map(((_key, value, _)) =>
                     <div
                       key={_key |> string_of_int}
                       onClick={
                         _event => {
                           ReactEvent.Mouse.preventDefault(_event);
                           setSearchKey(_ => "");
                           multiSelectCB(_key, value, true);
                         }
                       }
                       className="px-3 py-2 hover:bg-grey-lighter">
                       {value |> str}
                     </div>
                   )
                |> Array.of_list
                |> React.array
              }
            </div>
          </div>
        </div> :
        React.null
    }
  </div>;
};

module Jsx2 = {
  let component =
    ReasonReact.statelessComponent("CurriculumEditor__SelectBox");

  let make = (~items, ~multiSelectCB, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~items, ~multiSelectCB, ()),
      children,
    );
};