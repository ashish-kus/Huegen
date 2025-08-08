// service_worker.js
const NATIVE_NAME = "huegen_firefox"; // native host name

let port = null;

function log(...args) {
  console.log("NativeFox:", ...args);
}

function connectNative() {
  try {
    port = chrome.runtime.connectNative(NATIVE_NAME);
    log("connected to native host", NATIVE_NAME);

    port.onMessage.addListener(onNativeMessage);
    port.onDisconnect.addListener(() => {
      log("native host disconnected");
      port = null;
      // Reconnect after a short delay
      setTimeout(connectNative, 1500);
    });
  } catch (e) {
    console.error("Native connection failed:", e);
    // Try again later
    setTimeout(connectNative, 3000);
  }
}

function onNativeMessage(msg) {
  // Message expected to be an object with color keys (HEX strings)
  log("native msg:", msg);
  if (!msg || typeof msg !== "object") return;

  // Try to update browser UI theme if available (Firefox supports theme.update)
  try {
    if (chrome.theme && chrome.theme.update) {
      // Chrome's theme API is not widely supported or reliable for dynamic updates;
      // this call will succeed on Firefox (webextension polyfilled) and fail gracefully elsewhere.
      chrome.theme.update(msg);
      log("applied browser theme (if supported)");
    } else if (
      typeof browser !== "undefined" &&
      browser.theme &&
      browser.theme.update
    ) {
      // for browsers exposing `browser`
      browser.theme
        .update(msg)
        .then(() => log("applied theme via browser.theme.update"))
        .catch(() => {});
    }
  } catch (e) {
    // ignore - not supported
  }

  // Broadcast palette to all tabs (so content script can style pages)
  broadcastPalette(msg);
}

async function broadcastPalette(palette) {
  try {
    let tabs = await chrome.tabs.query({});
    for (let t of tabs) {
      try {
        chrome.tabs.sendMessage(t.id, {
          type: "NATIVE_FOXCOLOR_PALETTE",
          palette,
        });
      } catch (e) {
        // Tab may not have content script or may be chrome://, ignore
      }
    }
  } catch (e) {
    console.error("broadcast error", e);
  }
}

// Allow manual request to fetch palette from native host
chrome.runtime.onMessage.addListener((req, sender, sendResponse) => {
  if (req && req.type === "REQUEST_NATIVE_PALETTE") {
    // send a one-off native message if appropriate (sendNativeMessage opens a new host process)
    try {
      chrome.runtime.sendNativeMessage(
        NATIVE_NAME,
        { cmd: "get_palette" },
        (response) => {
          sendResponse(response);
        },
      );
      return true; // will respond asynchronously
    } catch (e) {
      sendResponse({ error: e.toString() });
    }
  }
});

connectNative();
