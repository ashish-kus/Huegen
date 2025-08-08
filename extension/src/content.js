// content.js
const INJECT_STYLE_ID = "native-foxcolor-style";

function applyPalette(p) {
  if (!p) return;
  const root = document.documentElement;

  const bg = p.page_background || p.popup || p.frame || "#111111";
  const fg = p.page_text || p.popup_text || p.toolbar_text || "#e6e6e6";
  const accent = p.tab_selected || p.sidebar_highlight || "#3a7bd5";

  root.style.setProperty("--nf-bg", bg);
  root.style.setProperty("--nf-fg", fg);
  root.style.setProperty("--nf-accent", accent);

  try {
    document.documentElement.style.backgroundColor = bg;
    if (document.body) {
      document.body.style.backgroundColor = bg;
      document.body.style.color = fg;
    }
  } catch (e) {
    /* ignore */
  }

  // inject style rules
  let style = document.getElementById(INJECT_STYLE_ID);
  if (!style) {
    style = document.createElement("style");
    style.id = INJECT_STYLE_ID;
    document.head?.appendChild(style);
  }
  style.textContent = `
    :root {
      color-scheme: dark;
      --nf-bg: ${bg};
      --nf-fg: ${fg};
      --nf-accent: ${accent};
    }
    body, html {
      background: var(--nf-bg) !important;
      color: var(--nf-fg) !important;
    }
    a, button, input, textarea, select {
      color: var(--nf-fg) !important;
    }
    a { text-decoration-color: var(--nf-accent) !important; }
    img, video { opacity: 0.95; }
  `;
}

// Listen for palette messages from background
chrome.runtime.onMessage.addListener((msg, sender, sendResponse) => {
  if (msg && msg.type === "NATIVE_FOXCOLOR_PALETTE") {
    applyPalette(msg.palette);
  }
});
