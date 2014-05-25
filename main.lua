bot = {}
bot.name = "botname"
bot.password = "botpass"
--bot.host = "127.0.0.1"
--bot.port = 5000
bot.host = "142.4.200.80"
bot.port = 5005

RegisterEvent("Login", function(event)
    if event.success then
        event.conn:SendGo('')
    end
end)

RegisterEvent("Chat", function(event)
    if event.inbound and event.type == ChatPrivate then
        if event.message == '!help' or event.message == '!about' then
            event.conn:SendChatPrivate(event.player, 'test lua bot')
        elseif event.message == '!owner' then
            event.conn:SendChatPrivate(event.player, 'Owner: monkey')
        elseif event.message == '!shutdown' then
            mods = {"Dr Brain", "Ceiu", "Noldec", "CDB-Man", "monkey", "Rivel"}
            for i, name in ipairs(mods) do
                if event.player == name then
                    Shutdown()
                end
            end
        end
    end
end)
