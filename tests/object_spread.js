// @flow
type T = {
  a: number,
  ...{ b: string },
} & { c: boolean};
