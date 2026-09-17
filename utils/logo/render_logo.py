# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

"""
Render the ESP-Unchained boot logo onto a black screen of a given size.

The logo is `chain -- rune -- chain`, snapped open on the inner ends so the
Bluetooth rune breaks free. It is assembled from the native sprites in sprites/
(1 px = 1 art pixel), then scaled by the LARGEST INTEGER factor that fits the
screen (nearest-neighbour -- any other resampling destroys pixel art), centred,
and drawn on a solid background.

The chain length is set by `--links` (mid sections per side). With none given the
script chooses it automatically: it tries 0, 1, 2, ... links and keeps whichever
fills the screen best (biggest logo that still fits). Narrow/vertical screens
therefore tend to get 0 links -- a shorter, taller-scaled logo -- while wide
screens get a longer chain. 0 links is perfectly valid (one snapped link per side).

    python render_logo.py 240 135                 # auto links -> logo.png
    python render_logo.py --width 320 --height 170 -o boot.png
    python render_logo.py 135 240                  # tall screen -> likely 0 links
    python render_logo.py 296 240 --margin 0.08    # leave an 8% border
    python render_logo.py 480 320 --links 2        # force 2 mid sections per side
    python render_logo.py 512 256 --transparent    # transparent background (RGBA)

Screen sizes are the two positional args (width height) or --width/--height.
"""

from PIL import Image
import argparse
import os

HERE = os.path.dirname(os.path.abspath(__file__))
SPRITES = os.path.join(HERE, "sprites")

GAP = 4          # native px between each snapped end and the rune
MAX_LINKS = 64   # safety ceiling for the automatic search


def _load(name):
    return Image.open(os.path.join(SPRITES, name)).convert("RGBA")


def load_tiles():
    """(left_cap, mid, right_cap, right_broken, left_broken, rune)."""
    return (_load("part1_left.png"), _load("part2_mid.png"), _load("part3_right.png"),
            _load("part3_right_broken.png"), _load("part1_left_broken.png"), _load("rune.png"))


def _hcat(imgs, gap=0):
    """Lay images left-to-right, vertically centred, on a transparent strip."""
    w = sum(i.width for i in imgs) + gap * (len(imgs) - 1)
    h = max(i.height for i in imgs)
    out = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    x = 0
    for i in imgs:
        out.paste(i, (x, (h - i.height) // 2), i)
        x += i.width + gap
    return out


def build_logo(tiles, links):
    """The native (unscaled) logo.

    left_cap + mid*links + right_broken  >gap<  rune  >gap<  left_broken + mid*links + right_cap
    Both inner ends are snapped and face the rune. `links` may be 0 (one link per side).
    """
    lc, mid, rc, rb, lb, ru = tiles
    left_chain = _hcat([lc] + [mid] * links + [rb])
    right_chain = _hcat([lb] + [mid] * links + [rc])
    return _hcat([left_chain, ru, right_chain], gap=GAP)


def native_size(tiles, links):
    """Logo width/height for a link count, without building the image."""
    lc, mid, rc, rb, lb, ru = tiles
    left_w = lc.width + mid.width * links + rb.width
    right_w = lb.width + mid.width * links + rc.width
    w = left_w + GAP + ru.width + GAP + right_w
    h = max(t.height for t in tiles)
    return w, h


def fit_scale(size, screen_w, screen_h, margin=0.0):
    """Largest integer scale so the logo fits within the screen (minus margin)."""
    w, h = size
    avail_w = screen_w * (1 - 2 * margin)
    avail_h = screen_h * (1 - 2 * margin)
    return min(int(avail_w // w), int(avail_h // h))


def choose_links(tiles, screen_w, screen_h, margin=0.0):
    """Pick the link count whose scaled logo fills the screen best.

    Best = largest rendered area among counts that fit at >=1x; ties go to the
    longer chain. Falls back to 0 links if nothing fits (it is the smallest logo).
    """
    avail_w = screen_w * (1 - 2 * margin)
    best = None  # (area, links)
    best_links = 0
    for links in range(0, MAX_LINKS + 1):
        w, h = native_size(tiles, links)
        scale = fit_scale((w, h), screen_w, screen_h, margin)
        if scale >= 1:
            key = ((scale * w) * (scale * h), links)  # area, then prefer more links
            if best is None or key > best:
                best, best_links = key, links
        if w > avail_w:      # already too wide; more links can only be worse
            break
    return best_links


def render(screen_w, screen_h, margin=0.0, links=None, bg=(0, 0, 0), transparent=False):
    """Return (canvas, links, scale, logo_native_size, clipped, auto).

    transparent=True gives an RGBA image with a fully transparent background
    (the logo keeps its own alpha); otherwise the background is filled with `bg`.
    """
    tiles = load_tiles()
    auto = links is None
    if auto:
        links = choose_links(tiles, screen_w, screen_h, margin)
    logo = build_logo(tiles, links)
    scale = fit_scale(logo.size, screen_w, screen_h, margin)
    clipped = scale < 1
    if clipped:
        scale = 1  # screen smaller than the logo even at 1x: centre-crop
    big = logo.resize((logo.width * scale, logo.height * scale), Image.NEAREST)
    if transparent:
        canvas = Image.new("RGBA", (screen_w, screen_h), (0, 0, 0, 0))
    else:
        canvas = Image.new("RGB", (screen_w, screen_h), bg)
    canvas.paste(big, ((screen_w - big.width) // 2, (screen_h - big.height) // 2), big)
    return canvas, links, scale, logo.size, clipped, auto


def _hexcolor(s):
    s = s.lstrip("#")
    return tuple(int(s[i:i + 2], 16) for i in (0, 2, 4))


def to_c_header(img, var, argv_hint):
    """RGB565 C header, values byte-swapped into the panel's order.

    Emits `#define <VAR>_W/_H` and `static const uint16_t <var>[]`, ready to pass
    straight to display_blit (same byte order as the gfx GFX_SWAP colours).
    """
    img = img.convert("RGB")
    w, h = img.size
    px = img.load()
    vals = []
    for y in range(h):
        for x in range(w):
            r, g, b = px[x, y]
            c = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)   # RGB565
            vals.append("0x%04X" % (((c >> 8) | (c << 8)) & 0xFFFF))  # panel byte order
    body = ",\n".join("    " + ",".join(vals[i:i + 16]) for i in range(0, len(vals), 16))
    up = var.upper()
    return (
        "/*\n"
        " * Boot splash image -- GENERATED, do not edit.\n"
        " * Regenerate with:\n"
        " *   python render_logo.py %s\n"
        " * RGB565 in the panel's byte order (ready for display_blit).\n"
        " */\n"
        "#ifndef UI_%s_H\n"
        "#define UI_%s_H\n\n"
        "#include <stdint.h>\n\n"
        "#define %s_W %d\n"
        "#define %s_H %d\n\n"
        "static const uint16_t %s[%s_W * %s_H] = {\n"
        "%s\n"
        "};\n\n"
        "#endif /* UI_%s_H */\n"
        % (argv_hint, up, up, up, w, up, h, var, up, up, body, up)
    )


if __name__ == "__main__":
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("size", nargs="*", type=int, metavar="W H",
                    help="screen width and height in pixels")
    ap.add_argument("--width", type=int, help="screen width (instead of positional)")
    ap.add_argument("--height", type=int, help="screen height (instead of positional)")
    ap.add_argument("--margin", type=float, default=0.0,
                    help="fraction of each edge to keep clear, 0..0.49 (default 0)")
    ap.add_argument("--links", type=int, default=None,
                    help="mid chain sections per side; omit to choose automatically (0 is valid)")
    ap.add_argument("--bg", type=_hexcolor, default=(0, 0, 0), help="background RRGGBB (default 000000)")
    ap.add_argument("--transparent", action="store_true",
                    help="transparent background instead of --bg (writes an RGBA PNG)")
    ap.add_argument("--format", choices=("png", "c"), default="png",
                    help="output a PNG (default) or a C/RGB565 header (--format c)")
    ap.add_argument("--var", default="logo_img",
                    help="array name for --format c (default logo_img)")
    ap.add_argument("-o", "--out", default="logo.png")
    a = ap.parse_args()

    if a.width and a.height:
        sw, sh = a.width, a.height
    elif len(a.size) == 2:
        sw, sh = a.size
    else:
        ap.error("give a screen size: `render_logo.py W H` or --width W --height H")

    if not 0.0 <= a.margin < 0.5:
        ap.error("--margin must be in [0, 0.5)")
    if a.links is not None and a.links < 0:
        ap.error("--links must be >= 0")

    canvas, links, scale, (lw, lh), clipped, auto = render(sw, sh, a.margin, a.links, a.bg, a.transparent)
    if a.format == "c":
        hint = "%d %d --links %d --format c -o %s" % (sw, sh, links, a.out)
        open(a.out, "w", newline="\n").write(to_c_header(canvas, a.var, hint))
        print("%s  %s[%d*%d] RGB565   links=%d%s   logo x%d"
              % (a.out, a.var, sw, sh, links, " (auto)" if auto else "", scale))
    else:
        canvas.save(a.out)
        print("%s  %dx%d screen   links=%d%s   logo %dx%d native x%d = %dx%d   bg=%s"
              % (a.out, sw, sh, links, " (auto)" if auto else "", lw, lh, scale,
                 lw * scale, lh * scale, "transparent" if a.transparent else "#%02X%02X%02X" % a.bg))
    if clipped:
        print("  WARNING: screen smaller than the logo at 1x -- it is centre-cropped."
              " Use --links 0 or a larger screen.")
