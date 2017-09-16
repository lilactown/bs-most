exception Oh_no;

open Jest;

module Helpers = {
  let noOp _ => ();
  let combine coll el => Js.Array.push el coll |> noOp;
  let asyncExpectToEqual expected actual => Js.Promise.resolve Expect.(expect actual |> toEqual expected);
};

testPromise
  "empty"
  (
    fun _ => {
      let empty = Most.empty ();
      let success _ => Helpers.asyncExpectToEqual true true;
      Most.observe (fun _ => raise Oh_no) empty |> Js.Promise.then_ success
    }
  );

testAsync
  "just"
  (
    fun done_ => {
      open Expect;
      let success result => done_ (expect result |> toBe "asdf");
      Most.just "asdf" |> Most.observe success |> Helpers.noOp
    }
  );

testPromise
  "from"
  (
    fun _ => {
      let result = [||];
      let combine = Helpers.combine result;
      let success _ => Helpers.asyncExpectToEqual [|1, 2|] result;
      Most.from [|1, 2|] |> Most.observe combine |> Js.Promise.then_ success
    }
  );

testPromise
  "concat"
  (
    fun _ => {
      let result = [||];
      let combine = Helpers.combine result;
      let s1 = Most.from [|1, 2, 3|];
      let s2 = Most.from [|4, 5, 6|];
      let success _ => Helpers.asyncExpectToEqual [|1, 2, 3, 4, 5, 6|] result;
      Most.concat s2 s1 |> Most.observe combine |> Js.Promise.then_ success
    }
  );

testPromise
  "startWith"
  (
    fun _ => {
      let result = [||];
      let combine = Helpers.combine result;
      let s = Most.from [|1, 2, 3|];
      let success _ => Helpers.asyncExpectToEqual [|0, 1, 2, 3|] result;
      Most.startWith 0 s |> Most.observe combine |> Js.Promise.then_ success
    }
  );


