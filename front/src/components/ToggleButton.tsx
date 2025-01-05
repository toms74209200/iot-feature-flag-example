interface ToggleButtonProps {
  onChange: (checked: boolean) => void;
}

export const ToggleButton = ({ onChange }: ToggleButtonProps) => {
  return (
    <>
      <input
        type="checkbox"
        className="sr-only peer"
        onChange={(e) => onChange(e.target.checked)}
      />
      <div
        className={[
          "relative",
          "w-11",
          "h-6",
          "bg-gray-200",
          "peer-focus:outline-none",
          "peer-focus:ring-2",
          "peer-focus:ring-blue-300",
          "dark:peer-focus:ring-blue-800",
          "rounded-full",
          "peer",
          "peer-checked:after:translate-x-full",
          "peer-checked:after:border-white",
          "after:content-['']",
          "after:absolute",
          "after:top-[2px]",
          "after:start-[2px]",
          "after:bg-white",
          "after:border-gray-300",
          "after:border",
          "after:rounded-full",
          "after:h-5",
          "after:w-5",
          "after:transition-all",
          "peer-checked:bg-blue-600",
        ].join(" ")}
      ></div>
    </>
  );
};
