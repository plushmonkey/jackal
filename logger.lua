RegisterEvent("Login", function(event)
    if event.success then
        print("Logged in as " .. event.name)
    else
        print("Error logging in: " .. event.message)
    end
end)

RegisterEvent("Chat", function(event)
    if event.type == ChatArena then
        print("A: " .. event.message)
    elseif event.type == ChatPublic or event.type == ChatPublicMacro then
        print(event.player .. "> " .. event.message)
    elseif event.type == ChatPrivate then
        print("P: " .. event.player .. "> " .. event.message)
    elseif event.type == ChatChannel then
        print(event.channel .. ":" .. event.message)
    elseif event.type == ChatFrequency then
        print("T: " .. event.player .. "> " .. event.message)
    end
end)

RegisterEvent("Enter", function(event)
    print(event.player .. " entered the arena.")
end)

RegisterEvent("Leave", function(event)
    print(event.player .. " left the arena.")
end)

RegisterEvent("Kill", function(event)
    print(event.killer .. " killed " .. event.killed)
end)

RegisterEvent("ShipFreqChange", function(event)
    print(event.player .. " changed to ship " .. event.ship)
end)
