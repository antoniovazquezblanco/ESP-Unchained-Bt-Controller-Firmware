# Boot logo

Pixel-art "Bluetooth Unchained" boot logo: a chain running in from both sides,
snapped open so the Bluetooth bind rune breaks free in the middle.

## Generate a logo PNG

`render_logo.py` draws the logo centred on a black screen of a given size. It
picks the **largest integer scale** that fits (nearest-neighbour -- the only
correct way to scale pixel art), centres it, and writes a PNG.

    python render_logo.py 240 135                  # auto links -> logo.png
    python render_logo.py --width 320 --height 170 -o boot.png
    python render_logo.py 135 240                   # tall screen -> likely 0 links
    python render_logo.py 296 240 --margin 0.08     # keep an 8% border
    python render_logo.py 480 320 --links 2         # force 2 mid sections per side

The chain length adapts to the screen. With no `--links`, the script tries 0, 1,
2, ... mid sections per side and keeps whichever fills the screen best (largest
logo that still fits). Narrow/vertical screens tend to get **0 links** -- a
shorter logo that can scale up further -- while wide screens get a longer chain.
**0 links is valid**: one snapped link per side flanking the rune.

Options:

| flag | meaning |
|------|---------|
| `W H` or `--width/--height` | screen size in pixels (required) |
| `--margin` | fraction of each edge kept clear, `0..0.49` (default `0`) |
| `--links`  | mid chain sections per side; omit to choose automatically (`0` is valid) |
| `--bg`     | background colour `RRGGBB` (default `000000`) |
| `--transparent` | transparent background instead of `--bg` (writes an RGBA PNG) |
| `-o`       | output path (default `logo.png`) |

If the screen is smaller than the logo even at 1x, the logo is centre-cropped and
a warning is printed -- use `--links 0` or a larger screen.

## The art

`sprites/` holds the canonical native art -- **1 px = 1 art pixel, the source of
truth**. Scale only by integer factors, nearest-neighbour.

| sprite | size | role |
|--------|------|------|
| `part1_left.png`         | 16x10 | chain left end cap |
| `part2_mid.png`          | 17x10 | chain middle, repeats seamlessly |
| `part3_right.png`        | 15x10 | chain right end cap |
| `part3_right_broken.png` | 15x14 | snapped end, faces the rune from the left |
| `part1_left_broken.png`  | 16x14 | snapped end, faces the rune from the right |
| `rune.png`               | 17x31 | Bluetooth bind rune (blue) |

The chain is a 6-tone blue-grey chrome ramp; the rune is a separate blue ramp
with a **blue (not black) outline** so it does not vanish on the black splash
background. The two snapped ends are deliberately *not* mirror images of each
other -- they tear at slightly different angles and scatter their shards
differently.

## Editing the art

All six sprites in `sprites/` are hand-authored native PNGs and are the sole
source of truth -- there are no generator scripts. Edit a PNG directly (1 px =
1 art pixel) and `render_logo.py` picks up the change on the next run. Keep the
6-tone chain ramp / blue rune ramp consistent, and remember the whole point is
integer, nearest-neighbour scaling.

## Note

Nothing here is wired into the firmware yet. `main/ui/splash_art.h` is still the
old 160x44 splash; exporting this logo to that format is future work.
