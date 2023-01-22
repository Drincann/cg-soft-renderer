# soft renderer

build:

- with fish shell:

```fish
fish ./tools/build.fish -o build -s main.cpp -open output.tga
```

- or by hand:

```bash
g++ --std=c++11 -g -o main main.cpp -lm
```

debug in vscode:

use the config in `.vscode/launch.json` this repo provides.
