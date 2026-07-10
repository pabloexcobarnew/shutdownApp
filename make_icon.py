#!/usr/bin/env python3
"""Generate a simple green 'power' icon (icon.ico) with no external deps.

Writes a multi-size ICO (32x32 + 16x16), 32-bit BGRA with alpha, drawing the
classic power symbol: a ring with a gap at the top and a vertical bar through
the gap.
"""
import math
import struct

# Power-green foreground; transparent background.
FG = (46, 204, 113)  # R, G, B


def render(size):
    """Return width*height list of (B, G, R, A) tuples, top-down."""
    px = [(0, 0, 0, 0)] * (size * size)
    cx = (size - 1) / 2.0
    cy = (size - 1) / 2.0
    scale = size / 32.0
    R = 9.5 * scale            # ring radius
    ring_t = 1.7 * scale       # ring half-thickness
    bar_hw = 1.4 * scale       # bar half-width
    bar_top = cy - 13.0 * scale
    bar_bot = cy - 1.0 * scale
    gap_hw = 2.0 * scale       # half-width of the top gap in the ring

    for y in range(size):
        for x in range(size):
            dx = x - cx
            dy = y - cy
            dist = math.hypot(dx, dy)
            on = False
            # ring, minus a vertical gap at the top (dy < 0, |dx| small)
            if abs(dist - R) <= ring_t and not (dy < 0 and abs(dx) <= gap_hw):
                on = True
            # vertical bar from near the top down to just above centre
            if abs(dx) <= bar_hw and bar_top <= y <= bar_bot:
                on = True
            if on:
                px[y * size + x] = (FG[2], FG[1], FG[0], 255)
    return px


def ico_image(size):
    """Return the BMP-in-ICO byte blob for one square image."""
    px = render(size)
    # BITMAPINFOHEADER: height is doubled to cover XOR + AND masks.
    header = struct.pack("<IiiHHIIiiII",
                         40, size, size * 2, 1, 32, 0, 0, 0, 0, 0, 0)
    # XOR (color) mask, bottom-up rows.
    xor = bytearray()
    for y in range(size - 1, -1, -1):
        for x in range(size):
            b, g, r, a = px[y * size + x]
            xor += bytes((b, g, r, a))
    # AND (transparency) mask: 1bpp, rows padded to 4 bytes. Alpha handles
    # transparency for 32-bit icons, so leave it all zero (opaque).
    row_bytes = ((size + 31) // 32) * 4
    and_mask = bytes(row_bytes * size)
    return header + bytes(xor) + and_mask


def main():
    sizes = [32, 16]
    images = [ico_image(s) for s in sizes]

    out = bytearray()
    out += struct.pack("<HHH", 0, 1, len(sizes))  # ICONDIR
    offset = 6 + 16 * len(sizes)
    for s, img in zip(sizes, images):
        w = 0 if s == 256 else s
        out += struct.pack("<BBBBHHII",
                           w, w, 0, 0, 1, 32, len(img), offset)  # ICONDIRENTRY
        offset += len(img)
    for img in images:
        out += img

    with open("icon.ico", "wb") as f:
        f.write(out)
    print("wrote icon.ico (%d bytes)" % len(out))


if __name__ == "__main__":
    main()
