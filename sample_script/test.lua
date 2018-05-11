twait = 3000
tdelay = 13000

print("twait = ", twait, ", tdelay = ", tdelay)

x, y = readcursor();

print("x = ", x, ", y = ", y);

print("Sleeping ...")

sleep(twait)

print("Go")



while true do
	setcursor(x, y);

	print("Left click");
	left_click();

	print("sleep for ", tdelay);
	sleep(tdelay);

	print("Enter");
	key("VK_RETURN");

	print("sleep for ", tdelay);
	sleep(tdelay);
end



