# quantization_trie
Written as an assignment - II semester MIM UW.

Stores a collection of "valid" {0,1,2,3}* "histories", some of them additionally assigned an energy level.
Accepts the following commands:
DECLARE history – Declares every prefix of the given history valid.

REMOVE history – Declares every prefix of the given history invalid.

VALID history – Checks, if the given history is valid.

ENERGY history energy – For a valid history, assigns or updates the energy value.

ENERGY history – For a valid history with an assigned energy value, prints the value.

EQUAL history_a history_b – For two valid histories:
(a) If one has an assigned energy, sets the other's equal and links them in an equivalence relation
(b) If both have an assigned energy, sets  the energies to their average and links them in an equivalence relation.
