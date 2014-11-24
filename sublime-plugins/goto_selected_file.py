import sublime, sublime_plugin
import re

# Opens the 'goto file' overlay with selected text as initial search
# Will also inspect the selection (or current line if no selection) and pull out
# header paths from #include directives
class GotoSelectedFileCommand(sublime_plugin.WindowCommand):

    def run(self):
        view = self.window.active_view()
        sel  = view.sel()

        searchText = ''
        if len(sel) and len(sel[0]):
            searchText = view.substr(sel[0])
        else:
            # grab the whole line text
            searchRegion = view.line(sel[0])
            searchText   = view.substr(searchRegion)

            # test for #include & extract path
            match = re.match('#\s*include\s*["<]([^">]+)[">]', searchText)
            if match:
                searchText = match.group(1)
            else:
                searchText = ''

        self.window.run_command("show_overlay", {"overlay":    "goto",
                                                 "show_files": True,
                                                 "text":       searchText})
