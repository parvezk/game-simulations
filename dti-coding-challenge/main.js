/***** Challenge 2 *****/

let set, size, id;
let posUpdates = [];
let positions = [];
let a = 0;

const main = (N, M, T) => {
  const enumerate = (N, M) => {
    set = Array.from(Array(N), (x, index) => {
      id = index < M ? `M${index + 1}` : 0;

      if (id) {
        const obj = {};
        obj[id] = 0;
        posUpdates.push(obj);
      }
      return id;
    });
    size = N - 1;
  };

  function isNextMoveValid(val, index) {
    if (set[index + 1] == 0) 
      return set[index];
    else if (set[index + 1] == undefined) 
      if (set[0] == 0) 
        return set[size];
  }

  function getRandomIntInclusive(min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min + 1)) + min;
  }

  function elemIs(x) {
    return Object.is.bind(Object, x);
  }

  let rand;
  const simulation = T => {
    let movables = [];

    for (i = 0; i < T; i++) {
      for (const [index, value] of set.entries()) {
        if (value) {
          if (isNextMoveValid(value, index)) 
            movables.push(value);
        }
      }

      rand = getRandomIntInclusive(0, movables.length - 1);
      let ind;
      let movableItem = movables[rand];
      ind = set.findIndex(elemIs(movableItem));

      for (const [index, value] of posUpdates.entries()) {
        const x = Object.keys(value)[0];
        if (x == movableItem) {
          value[movableItem] += index;
        }
      }

      if (ind < size) {
        set[ind + 1] = movableItem;
        set[ind] = 0;
      } else {
        set[0] = movableItem;
        set[size] = 0;
      }
      movables = [];
    }
  };
  enumerate(N, M);
  simulation(T);

  for (const [index, value] of Array.from(posUpdates.entries())) {
    a = Object.values(value)[0] / M;
    positions.push(a);
  }
};

main(25, 10, 50);

const sum = positions.reduce(function(sum, value) {
  return sum + value;
}, 0);

const avg = sum / positions.length;

Array.prototype.standardDeviation = function sd() {
  const average = data =>
    data.reduce((sum, value) => sum + value) / data.length;
  return Math.sqrt(average(this.map(value => (value - average(this)) ** 2)));
};
// Outputs
console.log(sum);
console.log(avg);
console.log(positions.standardDeviation());