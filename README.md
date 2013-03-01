# Mochi

The programming language

## Examples

```
Deck (name)
  + init
    @cards: []

  + add_card card
    @cards << card

  + play
    cards.each -> card
      play card

  - private_method
    out "Woot"

Card (front back)
  + play
    out @front
    guess: in.chomp
    if @back == guess
      out "Correct"
    else
      out "Incorrect. The answer was #{@back}

deck: Deck name: "Japanese" # indenting after a variable creates a scope
  add_card Card front "Cat" back "猫"
  add_card Card front "Dog" back "犬"
  play

deck.play # send a message

+ play thing
  out "woot"

play deck

+ fib n
  if n == 0 || n == 1, n
  else, (fib n - 1) + (fib n - 2)

fib 5
```

## License

Mochi is released under the [MIT License](http://opensource.org/licenses/MIT)
