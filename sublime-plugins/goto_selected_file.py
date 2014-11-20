import sublime, sublime_plugin

# Opens the 'goto file' overlay with selected text as initial search
class GotoSelectedFileCommand(sublime_plugin.WindowCommand):
    def run(self):
        view = self.window.active_view()
        sel  = view.sel()

        searchText = ''
        if len(sel):
           searchText = view.substr(sel[0])
           
        self.window.run_command("show_overlay", {"overlay":    "goto",
                                                 "show_files": True,
                                                 "text":       searchText})
