-- left click at current cursor position, and type all available type strings.
left_click();

type("1234567890-=\\");
key("VK_RETURN");

type("!@#$%^&*()_+|");
key("VK_RETURN");

type("qwertyuiop[]");
key("VK_RETURN");

type("QWERTYUIOP{}");
key("VK_RETURN");

type("asdfghjkl;'");
key("VK_RETURN");

type("ASDFGHJKL:\"");
key("VK_RETURN");

type("zxcvbnm,./");
key("VK_RETURN");

type("ZXCVBNM<>?");
key("VK_RETURN");

type("space test\tspace");
key("VK_RETURN");

print("done");
