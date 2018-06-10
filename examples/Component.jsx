// @flow
import React, { type ComponentType } from "react";
import classnames from "classnames";
import style from "./Text.scss";

export type Props = {|
  text: string,
  oneLiner?: boolean,
  size?: "extraSmall" | "small" | "medium" | "large" | "extraLarge",
  color?:
    | "secondary"
    | "secondaryDark"
    | "pewterBlueDark"
    | "pewterBlue"
    | "sunsetRed"
    | "nearWhite",
  isBold?: boolean,
  className?: string,
|};

export const Text: ComponentType<Props> = ({
  text,
  className,
  oneLiner = false,
  size = "medium",
  color = "secondary",
  isBold = false,
}) => (
  <div
    className={classnames(style.my, className, {
      [style.myOneLiner]: oneLiner,
      [style.myBold]: isBold,
      [style.myColorPewterBlue]: color === "pewterBlue",
      [style.myColorPewterBlueDark]: color === "pewterBlueDark",
      [style.myColorSecondary]: color === "secondary",
      [style.myColorSecondaryDark]: color === "secondaryDark",
      [style.myColorSunsetRed]: color === "sunsetRed",
      [style.myColorNearWhite]: color === "nearWhite",
      [style.mySizeExtraLarge]: size === "extraLarge",
      [style.mySizeExtraSmall]: size === "extraSmall",
      [style.mySizeLarge]: size === "large",
      [style.mySizeMedium]: size === "medium",
      [style.mySizeSmall]: size === "small",
    })}>
    {text}
  </div>
);
