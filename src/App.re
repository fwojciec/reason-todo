type todo = {
  id: string,
  text: string,
  completed: bool,
};

type state = {todos: list(todo)};

type action =
  | Add(string)
  | Check(string)
  | Delete(string);

/* toString = ReasonReact.stringToElement */

/* todoId = use Uuid.V4.uuidv4() instead */

let newTodo = text => {
  let todoId = Uuid.V4.uuidv4();
  {id: todoId, completed: false, text};
};

let check = (id, todos) =>
  List.map(t => t.id === id ? {...t, completed: !t.completed} : t, todos);

let delete = (id, todos) => List.filter(t => t.id !== id, todos);

let valueFromEvent = event => ReactEvent.Form.target(event)##value;

module Input = {
  [@react.component]
  let make = (~onSubmit) => {
    let (value, onChange) = React.useState(() => "");

    <input
      className="new-todo"
      type_="text"
      value
      placeholder="What do you want to do?"
      onChange={event => onChange(ReactEvent.Form.target(event)##value)}
      onKeyDown={event =>
        if (ReactEvent.Keyboard.key(event) == "Enter") {
          onSubmit(value);
          onChange(_ => "");
        }
      }
    />;
  };
};

module TodoItem = {
  [@react.component]
  let make = (~todo, ~onToggle, ~onDelete) => {
    <div className="item">
      <label>
        <input
          className="checkbox"
          type_="checkbox"
          checked={todo.completed}
          onChange={_event => onToggle()}
        />
        {React.string(todo.text)}
      </label>
      <button className="btn-delete" onClick={_event => onDelete()}>
        {React.string("Delete")}
      </button>
    </div>;
  };
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Add(text) => {todos: [newTodo(text), ...state.todos]}
        | Check(id) => {todos: check(id, state.todos)}
        | Delete(id) => {todos: delete(id, state.todos)}
        },
      {todos: []},
    );

  <div className="container">
    <h3> {React.string("Todo App")} </h3>
    <Input onSubmit={value => dispatch(Add(value))} />
    {List.map(
       todo =>
         <TodoItem
           key={todo.id}
           onToggle={() => dispatch(Check(todo.id))}
           onDelete={() => dispatch(Delete(todo.id))}
           todo
         />,
       state.todos,
     )
     |> Array.of_list
     |> React.array}
  </div>;
};