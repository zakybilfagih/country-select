module Search = {
  [@react.component]
  let make = () => {
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width="14"
      height="14"
      fill="currentcolor"
      viewBox="0 0 14 14">
      <path
        fill="#333"
        fillRule="evenodd"
        d="M6 11c1.02 0 1.967-.305 2.758-.829l3.302 3.299 1.41-1.41-3.299-3.302A5 5 0 106 11zm3.2-5a3.2 3.2 0 11-6.4 0 3.2 3.2 0 016.4 0z"
        clipRule="evenodd"
      />
    </svg>;
  };
};

module Triangle = {
  [@react.component]
  let make = (~className=?, ~style=?) => {
    <svg
      ?className
      ?style
      xmlns="http://www.w3.org/2000/svg"
      width="8"
      height="8"
      fill="currentcolor"
      viewBox="0 0 8 8">
      <path
        fill="#333"
        fillRule="evenodd"
        d="M0 2h8L4 7 0 2z"
        clipRule="evenodd"
      />
    </svg>;
  };
};

module Loading = {
  [@react.component]
  let make = () => {
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width="18"
      height="18"
      viewBox="0 0 24 24">
      <path
        d="M12 1a11 11 0 1011 11A11 11 0 0012 1zm0 19a8 8 0 118-8 8 8 0 01-8 8z"
        opacity="0.25"
      />
      <path
        d="M12 4a8 8 0 017.89 6.7 1.53 1.53 0 001.49 1.3 1.5 1.5 0 001.48-1.75 11 11 0 00-21.72 0A1.5 1.5 0 002.62 12a1.53 1.53 0 001.49-1.3A8 8 0 0112 4z">
        <animateTransform
          attributeName="transform"
          dur="0.75s"
          repeatCount="indefinite"
          type_="rotate"
          values="0 12 12;360 12 12"
        />
      </path>
    </svg>;
  };
};
